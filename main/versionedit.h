#include <string>
#include <vector>
#include <set>
#include <map>
#include "memory/skiplist.h"
#include "utils/env.h"
#pragma once

namespace leveldb {

class VersionSet;
class Compacter;
struct FileMetaData {
    int refs;
    std::string smallest;
    std::string largest;
    int file_size;
    int number;
    int allowed_seeks;
    bool mayInFile(std::string& user_key){
    	return user_key >= smallest && user_key <= largest;
    }
    FileMetaData() : refs(0), file_size(0) { }
};
class VersionEdit{
public:
    VersionEdit() {clear();}
    void clear();
private:
    friend class VersionSet;
    friend class Compacter;
    typedef std::set< std::pair<int, uint64_t> > DeletedFileSet;

    std::string comparator_;
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
    std::vector< std::pair<int, FileMetaData> > new_files_;
    std::vector<SkipList*> deleted_lists_;
};

}