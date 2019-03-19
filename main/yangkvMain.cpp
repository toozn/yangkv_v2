#include "yangkvMain.h"
#include "memory/writer.h"
#include "versionset.h"
#include "memory/compacter.h"
#include <unistd.h>
#include <iostream>

namespace leveldb {

std::string NullStr = "";

YangkvMain::YangkvMain(){}

void* WriterBegin(void* instance) {
    pthread_detach(pthread_self());
    assert(instance != nullptr);
    ((Writer*)instance)->workRound();
    return nullptr;
}

void* CompacterBegin(void* instance) {
    pthread_detach(pthread_self());
    assert(instance != nullptr);
    ((Compacter*)instance)->workRound();
    return nullptr;
}

void YangkvMain::init() {
    idx_ = 1;
    bg_lock_ = new CondLock();
    env_ = new Env();
    set_ = new VersionSet(this, bg_lock_);
    for (int id = 0; id < kMaxWriter; id++) {
        
        writer_args_[id] = new WriterConfig(id);
        writer_[id] = new Writer(set_, writer_args_[id]);
        pthread_t tid;
        pthread_create(&tid, nullptr, WriterBegin, (void*)writer_[id]);
    }
    {
        compacter_args_ = new CompacterConfig();
        compacter_ = new Compacter(set_, bg_lock_, env_, compacter_args_);
        pthread_t tid;
        pthread_create(&tid, nullptr, CompacterBegin, (void*)compacter_);
    }

    sleep(1);
}

Status YangkvMain::setKey(std::string& key, std::string& value, bool del_flag) {
	uint64_t id = idx_++;
    int writerID = strHash(key, kSeedForWriter) % kMaxWriter;
    auto queue = &writer_[writerID]->queue_;
    assert(queue != nullptr);
    MemEntry entry = MemEntry(key, value, id, del_flag);
    //entry.debug();
	queue->push(entry);

    return Status::OK();
}

Status YangkvMain::delKey(std::string& key) {
	return setKey(key, NullStr, true);
}

Status YangkvMain::getValue(std::string& key, std::string* value) {
    uint64_t seq = idx_;
    int writerID = strHash(key, kSeedForWriter) % kMaxWriter;
    Status s;
    //search in message queue
    bool result = writer_[writerID]->queue_.search(key, seq, value, &s);
    if (result) {
        return s;
    }
    //printf("%s %d\n", key.c_str(), __LINE__);
    //search in activelist
    result = writer_[writerID]->getEntry(key, seq, value, &s);
    if (result) {
        return s;
    }
    else {
        //search in frozenlist and sstable
        Version* curr_ = set_->Current();
        result = curr_->Get(key, seq, value);
        if (result) {
            return Status::OK();
        }
        else {
            return Status::NotFound();
        }
        
    }
}


void YangkvMain::stop() {
    for (int id = 0; id < kMaxWriter; id++) {
        writer_args_[id]->stopFLAG = true;
    }
    compacter_args_->stopFLAG = true;
    sleep(2);
    puts("YangKV Stopped!");
}

}