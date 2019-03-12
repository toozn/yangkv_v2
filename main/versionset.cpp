#include "versionset.h"

namespace leveldb {

void Version::ref(){
    ref_++;
}
void Version::unref(){
    assert(this != &set_->dummyVersion_ && ref_);
    ref_--;
    if (ref_ == 0) {
        for (int i = 0; i < kMaxWriter; i++) {
            for (auto it: maintainList_[i]) {
                delete it;
            }
        }
        nxt_->pre_ = pre_;
        pre_->nxt_ = nxt_;
        delete this;
    }
}

Status Version::apply(Version* curr, VersionEdit* edit) {
    return Status::OK();
}

bool Version::Get(std::string& key, uint64_t seq, std::string* value, Status* s) {
    Slice slice;
    for (int i = 0; i < kMaxWriter; i++) {
        for (auto list: maintainList_[i]) {
            Status ss = list->get(key, slice);
            if (ss.ok()) {
                MemEntry entry;
                DecodeMemEntry(slice, entry);
                if (entry.value_type == kDeleted) {
                    *s = Status::NotFound();
                    return true;
                }
                else {
                    *s = Status::OK();
                    value->assign(entry.value.data(), entry.value.size());
                    return true;
                }
                
            }
        }
    }
    vector<FileMetaData*> search_list;
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

void VersionSet::addVersion(Version* v) {
    v->pre_ = dummyVersion_.pre_;
    v->nxt_ = &dummyVersion_;
    v->nxt_->pre_ = v->pre_;
    v->pre_->nxt_ = v->nxt_;
}

Status VersionSet::applyEdit(VersionEdit* edit) {
    Version* v = new Version();
    v->apply(current_, edit);
    addVersion(v);
    return Status::OK();
}

}