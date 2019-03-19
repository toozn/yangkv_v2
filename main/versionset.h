#include "versionedit.h"
#include "utils/env.h"
#include <set>
#include <vector>

#pragma once

namespace leveldb {
class VersionSet;
class YangkvMain;
class CondLock;
class Compacter;

class Version{
public:
    Version(){}
    Version(VersionSet* set) {
        set_ = set;
        ref_ = 0;
        list_count = 0;
        pthread_rwlock_init(&rwlock_, NULL);
    }
    void Ref();
    void Unref();
    bool Get(std::string& key, uint64_t seq, std::string* value);
    bool isFull() {return list_count >= kMaxFrozenList;}
    void AppendList(SkipList* list, int idx);
    void Debug();
    Status apply(Version* curr, VersionEdit* edit);
    Version* next_;
    Version* prev_;
    int ref_;
private:
    friend class VersionSet;
    friend class Compacter;
    friend class VersionEdit;
    VersionSet* set_;
    std::vector<SkipList*>list_[kMaxWriter];
    uint32_t list_count;
    std::vector<FileMetaData*>files_[kMaxLevel];
    pthread_rwlock_t rwlock_;
};

class VersionSet{
public:
    VersionSet(YangkvMain* db, CondLock* lock);
    void AppendVersion(Version* v);
    void AppendFrozenList(SkipList* list, int idx);
    Version* Current() {return current_;}
    class Builder;
private:
    friend class YangkvMain;
    friend class Version;
    friend class Compacter;
    Version* current_;
    Version dummy_versions_;
    YangkvMain* db_;
    CondLock* compact_lock_;
    Compacter* compacter_;
private:
};

}