#include "writer.h"
#include "main/versionset.h"
#include "utils/config.h"
#include <unistd.h>

namespace yangkv {

class VersionSet;

void Writer::workRound() {
    int id = config_->writerID;
    printf("Writer%d begin to work\n", id);
    while (config_->stopFLAG == false) {
        mayInsertMessage();
    }
    printf("Quit Writer %d!\n", id);
}

Writer::Writer(VersionSet* set, WriterConfig* config, Env* env) {
    set_ = set;
    env_ = env;
    config_ = config;
    list_ = new SkipList();
    queue_ = new MessageQueue(config_->writerID, config_->logID, env_);
    pthread_rwlock_init(&rwlock_, NULL);
}

Writer::~Writer() {
    delete list_;
    delete queue_;
    delete config_;
}

void Writer::mayInsertMessage() {
    pthread_rwlock_wrlock(&rwlock_);
    while (queue_->isEmpty() == false) {
        list_->insert(queue_->getFront());
        queue_->pop();
        if (list_->size() >= kMaxActiveListSize) {
            set_->AppendFrozenList(list_, config_->writerID);
            list_ = new SkipList();
        }
    }
    pthread_rwlock_unlock(&rwlock_);
}

bool Writer::getEntry(std::string& key, uint64_t seq, std::string* value, Status* s) {
    pthread_rwlock_rdlock(&rwlock_);
    Slice slice;
    Status ss = list_->get(key, slice);
    pthread_rwlock_unlock(&rwlock_);
    if (ss.isNotFound()) {
        return false;
    }
    MemEntry entry;
    DecodeMemEntry(slice, entry);
    if (entry.seq_num > seq) {
        return false;
    }
    if (entry.value_type == kDeleted) {
        *s = Status::NotFound();
        return true;
    }
    else {
        *s = Status::OK();
        value->assign(entry.value.data(), entry.value.size());
    }
    return true;
}

/*
void* workRound(void* arg_) {
    pthread_detach(pthread_self());
    auto arg = (WriterConfig*)arg_;
    auto writer = (Writer*)arg->instance;
    while (arg->stopFLAG == false) {
        writer->mayInsertMessage();
        usleep(10);
    }
    printf("Writer Quit!\n");
    return 0;
}
*/

}