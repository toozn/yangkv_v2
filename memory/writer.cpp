#include "writer.h"
#include "compacter.h"

namespace leveldb {

class compacter;

Writer::Writer(Compacter* compacter) {
    list_ = new SkipList();
    compacter_ = compacter;
    queue_ = new MessageQueue();
    pthread_rwlock_init(&rwlock_, NULL);
}

Writer::~Writer() {
    delete list_;
}

void Writer::mayInsertMessage() {
    pthread_rwlock_wrlock(&rwlock_);
    while (queue_->isEmpty() == false) {
        
        MemEntry entry = queue_->getFront();
        list_->insert(entry);
        queue_->pop();
        //printf("Success Insert! KEY:%s VALUE:%s ID:%lld\n", msg->key.c_str(), msg->value.c_str(), msg->id);
        if (list_->size() >= kMaxListSize) {
            cout << "???" << endl;
            compacter_->pushList(list_);
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

void* workRound(void* arg_) {
    pthread_detach(pthread_self());
    auto arg = (WriterConfig*)arg_;
    auto writer = (Writer*)arg->instance;
    while (arg->stopFLAG == false) {
        writer->mayInsertMessage();
    }
    printf("Writer Quit!\n");
    return 0;
}

}