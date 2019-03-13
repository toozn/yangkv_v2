#include "memory/writer.h"
#include "memory/compacter.h"
#include "versionedit.h"
#include <set>
#include <vector>

#pragma once

namespace leveldb {

class VersionSet;
class yangkvMain;

class Version{
public:
    Version(){ ref_ = 1; }
    Version(VersionSet* set) {
        set_ = set;
        ref_ = 1;
    }
    void Ref();
    void Unref();
    bool Get(std::string& key, uint64_t seq, std::string* value, Status* s);
    Status apply(Version* curr, VersionEdit* edit);
    Version* nxt_;
    Version* pre_;
private:
    friend class VersionSet;
    friend class VersionEdit;
    VersionSet* set_;
    int ref_;
    vector<SkipList*>list_[kMaxWriter];
    vector<FileMetaData*>files_[kMaxLevel];
};

class VersionSet{
public:
    VersionSet(){
        dummyVersion_ = Version();
        current_ = &dummyVersion_;
    }
    class Builder;
private:
    friend class yangkvMain;
    friend class Version;
    Version* current_;
    Version dummyVersion_;
    yangkvMain* db_;
private:
};

}