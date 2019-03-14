#include "message_queue.h"
#include "utils/config.h"
#include "skiplist.h"
#include "utils/condlock.h"
#pragma once


namespace leveldb {

class YangkvMain;
class VersionSet;
class VersionEdit;

class Compacter {
public:
    Compacter(YangkvMain* db, CondLock* lock){
        db_ = db;
        lock_ = lock;
    }
    ~Compacter(){}
    void* compactRound(void* arg_);
    bool inCompact() {return false;}
    VersionEdit* calEdit();

private:
    friend class YangkvMain;
    friend class VersionEdit;
    friend class VersionSet;
    YangkvMain* db_;
    CondLock* lock_;
    int list_idx;
};

}