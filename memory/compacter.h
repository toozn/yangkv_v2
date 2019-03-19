#include "message_queue.h"
#include "utils/config.h"
#include "skiplist.h"
#include "utils/condlock.h"
#include "utils/env.h"
#pragma once


namespace leveldb {

class YangkvMain;
class VersionSet;
class VersionEdit;
class CompacterConfig;



class Compacter {
public:
    Compacter(VersionSet* set, 
              CondLock* lock, 
              Env* env, 
              CompacterConfig* config){
        set_ = set;
        lock_ = lock;
        env_ = env;
        config_ = config;
        inCompact_ = false;
    }
    ~Compacter(){}
    void workRound();
    bool inCompact() {return inCompact_;}
    VersionEdit* calEdit();

private:
    friend class YangkvMain;
    friend class VersionEdit;
    friend class VersionSet;
    CompacterConfig* config_;
    VersionSet* set_;
    CondLock* lock_;
    bool inCompact_;
    Env* env_;
};

}