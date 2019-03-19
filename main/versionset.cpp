#include "versionset.h"
#include "utils/hash_helper.h"
#include "utils/condlock.h"
#include <unistd.h>
namespace leveldb {
void Version::Ref(){
    ref_++;
}
void Version::Unref(){
    assert(this != &set_->dummy_versions_ && ref_);
    assert(ref_ >= 1);
    ref_--;
    if (ref_ == 0) {
        for (int i = 0; i < kMaxWriter; i++) {
            for (auto it: list_[i]) {
                it->unref();
            }
        }
        next_->prev_ = prev_;
        prev_->next_ = next_;
        delete this;
    }
}

Status Version::apply(Version* curr, VersionEdit* edit) {
    return Status::OK();
}

void Version::AppendList(SkipList* list, int idx) {
    pthread_rwlock_wrlock(&rwlock_);
    list_[idx].push_back(list);
    //list->Debug();
    pthread_rwlock_unlock(&rwlock_);
}

bool Version::Get(std::string& key, uint64_t seq, std::string* value) {
    Slice slice;
    int writerID = strHash(key, kSeedForWriter) % kMaxWriter;
    pthread_rwlock_rdlock(&rwlock_);
    int sz = list_[writerID].size();
    for (int i = sz - 1; i >= 0; i--) {
        SkipList* list = list_[writerID][i];
        Status ss = list->get(key, slice);
        if (ss.ok()) {
            pthread_rwlock_unlock(&rwlock_);
            MemEntry entry;
            DecodeMemEntry(slice, entry);
            if (entry.value_type == kDeleted) {
                return false;
            }
            else {
                value->assign(entry.value.data(), entry.value.size());
                return true;
            }
            
        }
    }
    pthread_rwlock_unlock(&rwlock_);
    std::vector<FileMetaData*> search_list;
    for (int lv = 0; lv < kMaxLevel; lv++) {
        for (auto file: files_[lv]) {
            if (file->mayInFile(key)) {
                search_list.push_back(file);
                if (lv == 0) continue; // level 0 has overlapped files
                else break; //for level 1 and above, only one file in each level may contain the key
            }
        }
    }

    for (auto& file: search_list) {
        ;
    }
    return false;
}

void Version::Debug() {
    printf("-----------------------\n");
    printf("ref: %d\n", ref_);
    for (int i = 0; i < kMaxWriter; i++) {
        printf("%d\n", (int)list_[i].size());
    }
    printf("-----------------------\n");
}

class VersionSet::Builder {
private:
    // Helper to sort by v->files_[file_number].smallest
    struct BySmallestKey {

    bool operator()(FileMetaData* f1, FileMetaData* f2) const {
        int r = f1->smallest < f2->smallest;
        if (r != 0) {
          return (r < 0);
        } else {
          // Break ties by file number
          return (f1->number < f2->number);
        }
    }
};

  typedef std::set<FileMetaData*, BySmallestKey> FileSet;
  struct LevelState {
    std::set<uint64_t> deleted_files;
    FileSet* added_files;
  };
  VersionSet* vset_;
  Version* base_;
  LevelState levels_[kMaxLevel];
  std::set<SkipList*> deleted_lists;

 public:
  // Initialize a builder with the files from *base and other info from *vset
  Builder(VersionSet* vset, Version* base)
      : vset_(vset),
        base_(base) {
    base_->Ref();
    for (int level = 0; level < kMaxLevel; level++) {
      levels_[level].added_files = new FileSet();
    }
  }

  ~Builder() {
    for (int level = 0; level < kMaxLevel; level++) {
        const FileSet* added = levels_[level].added_files;
        std::vector<FileMetaData*> to_unref;
        to_unref.reserve(added->size());
        for (FileSet::const_iterator it = added->begin();
            it != added->end(); ++it) {
            to_unref.push_back(*it);
        }
        delete added;
        for (uint32_t i = 0; i < to_unref.size(); i++) {
            FileMetaData* f = to_unref[i];
            f->refs--;
            if (f->refs <= 0) {
              delete f;
            }
        }
    }
    base_->Unref();
  }

  // Apply all of the edits in *edit to the current state.
  void Apply(VersionEdit* edit) {
    // Update compaction pointers
    /*
    for (size_t i = 0; i < edit->compact_pointers_.size(); i++) {
      const int level = edit->compact_pointers_[i].first;
      vset_->compact_pointer_[level] =
          edit->compact_pointers_[i].second.Encode().ToString();
    }
    */
    // Delete files
    for (auto iter: edit->deleted_files_) {
      const int level = iter.first;
      const uint64_t number = iter.second;
      levels_[level].deleted_files.insert(number);
    }

    // Add new files
    for (size_t i = 0; i < edit->new_files_.size(); i++) {
      const int level = edit->new_files_[i].first;
      FileMetaData* f = new FileMetaData(edit->new_files_[i].second);
      f->refs = 1;
      f->allowed_seeks = static_cast<int>((f->file_size / 16384U));
      if (f->allowed_seeks < 100) f->allowed_seeks = 100;

      levels_[level].deleted_files.erase(f->number);
      levels_[level].added_files->insert(f);
    }
  }

  // Save the current state in *v.
  void SaveTo(Version* v) {
    BySmallestKey cmp;
    for (int level = 0; level < kMaxLevel; level++) {
      // Merge the set of added files with the set of pre-existing files.
      // Drop any deleted files.  Store the result in *v.
      const std::vector<FileMetaData*>& base_files = base_->files_[level];
      auto base_iter = base_files.begin();
      auto base_end = base_files.end();
      const FileSet* added = levels_[level].added_files;
      v->files_[level].reserve(base_files.size() + added->size());
      for (FileSet::const_iterator added_iter = added->begin();
           added_iter != added->end();
           ++added_iter) {
        // Add all smaller files listed in base_
        for (std::vector<FileMetaData*>::const_iterator bpos
                 = std::upper_bound(base_iter, base_end, *added_iter, cmp);
             base_iter != bpos;
             ++base_iter) {
          MaybeAddFile(v, level, *base_iter);
        }

        MaybeAddFile(v, level, *added_iter);
      }

      // Add remaining base files
      for (; base_iter != base_end; ++base_iter) {
        MaybeAddFile(v, level, *base_iter);
      }
    }
  }

  void MaybeAddFile(Version* v, int level, FileMetaData* f) {
    if (levels_[level].deleted_files.count(f->number) > 0) {
      // File is deleted: do nothing
    } else {
        std::vector<FileMetaData*>* files = &v->files_[level];
        if (level > 0 && !files->empty()) {
          // Must not overlap
          assert((*files)[files->size()-1]->largest < f->smallest);
        }
        f->refs++;
        files->push_back(f);
    }
  }
};

VersionSet::VersionSet(YangkvMain* db, CondLock* lock) {
    db_ = db;
    compact_lock_ = lock;
    dummy_versions_ = Version(this);
    dummy_versions_.ref_ = 1;
    dummy_versions_.prev_ = &dummy_versions_;
    dummy_versions_.next_ = &dummy_versions_;
    AppendVersion(new Version(this));
}

void VersionSet::AppendVersion(Version* v) {
    // Make "v" current
    assert(v->ref_ == 0);
    assert(v != current_);
    if (current_ != nullptr) {
        current_->Unref();
    }
    current_ = v;
    v->Ref();
    v->prev_ = dummy_versions_.prev_;
    v->next_ = &dummy_versions_;
    v->prev_->next_ = v;
    v->next_->prev_ = v;
}

void VersionSet::AppendFrozenList(SkipList* list, int idx) {
    assert(compact_lock_ != nullptr);
    assert(current_ != nullptr);
    compact_lock_->Lock();
    if (current_->isFull()) {
        compact_lock_->Wait();
    }
    current_->AppendList(list, idx);
    compact_lock_->Unlock();
}

}