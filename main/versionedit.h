#include <string>
#include <vector>
#include <set>
#include <map>
#include "memory/skiplist.h"
#include "utils/env.h"
#pragma once

namespace yangkv {

class VersionSet;
class Compacter;
struct FileMetaData {
    int refs;
    MemEntry smallest;
    MemEntry largest;
    uint64_t file_size;
    uint64_t number;
    int allowed_seeks;
    void ref() {
      refs++;
    }
    void unref() {
      assert(refs > 0);
      refs--;
      if (refs == 0) {
        //TODO
      }
    }
    bool mayInFile(std::string& user_key){
    	return user_key >= smallest.key.ToString() 
               && user_key <= largest.key.ToString();
    }
    void debug() {
        printf("ref:%d, smallest:%s, largest:%s, file_size:%d, number:%d\n",
            refs, smallest.key.data(), largest.key.data(), (int)file_size, (int)number);
    }
    FileMetaData() : refs(0), file_size(0) { }
};
class VersionEdit{
public:
    VersionEdit() {Clear();}
    void Clear();

  void SetLogNumber(uint64_t num, int writerid) {
    has_log_number_ = true;
    log_number_[writerid] = num;
  }
  void SetPrevLogNumber(uint64_t num) {
    has_prev_log_number_ = true;
    prev_log_number_ = num;
  }
  void SetNextFile(uint64_t num) {
    has_next_file_number_ = true;
    next_file_number_ = num;
  }
  void SetLastSequence(uint64_t seq) {
    has_last_sequence_ = true;
    last_sequence_ = seq;
  }
  /*
  void SetCompactPointer(int level, const std::string& key) {
    compact_pointers_.push_back(std::make_pair(level, key));
  }
  */

  // Add the specified file at the specified number.
  // REQUIRES: This version has not been saved (see VersionSet::SaveTo)
  // REQUIRES: "smallest" and "largest" are smallest and largest keys in file
  void AddFile(int level, uint64_t file,
               uint64_t file_size,
               const MemEntry& smallest,
               const MemEntry& largest) {
    FileMetaData* f = new FileMetaData();
    f->number = file;
    f->file_size = file_size;
    f->smallest = smallest;
    f->largest = largest;
    new_files_.push_back(std::make_pair(level, f));
  }

  // Delete the specified "file" from the specified "level".
  void DeleteFile(int level, uint64_t file) {
    deleted_files_.insert(std::make_pair(level, file));
  }

  void EncodeTo(std::string* dst) const;
  Status DecodeFrom(const Slice& src);

  std::string debug() const;
    typedef std::set< std::pair<int, uint64_t> > DeletedFileSet;
    uint64_t log_number_[kMaxWriter];
    uint64_t prev_log_number_;
    uint64_t next_file_number_;
    uint64_t last_sequence_;
    bool has_comparator_;
    bool has_log_number_;
    bool has_prev_log_number_;
    bool has_next_file_number_;
    bool has_last_sequence_;

    //sssstd::vector< std::pair<int, InternalKey> > compact_pointers_;
    DeletedFileSet deleted_files_;
    std::vector< std::pair<int, FileMetaData*> > new_files_;
    std::vector<SkipList*> deleted_lists_;
};

}