#include "message_queue.h"
#include "utils/config.h"
#include "skiplist.h"
#pragma once


namespace leveldb {

class yangkvMain;

class Compacter {
public:
    Compacter(){
        list_idx = 0;
        list_[0].clear(), list_[1].clear();
        pthread_cond_signal(&cond_);
    }
    ~Compacter(){}
    void pushList(SkipList*);
    void* compactRound(void*);
    void CompactList();
private:
    static const int kMaxFrozenListSize = 10;
    pthread_cond_t cond_;
    vector<SkipList*>list_[2];
    yangkvMain* main_;
    int list_idx;
    pthread_mutex_t lock_;
};

}