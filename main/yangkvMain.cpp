#include "yangkvMain.h"
#include "memory/writer.h"
#include "versionset.h"
#include <unistd.h>
void* writerRound(void* arg_) {
    pthread_detach(pthread_self());
    auto arg = (WriterConfig*) arg_;
    auto writer = arg->instance;
    int id = arg->writerId;
    printf("Worker id: %d begin to work\n", id);
    while (arg->stopFLAG == false) {
        writer->mayInsertMessage();
    }
    printf("Quit Writer %d!\n", id);
    return 0;
}
YangkvMain::YangkvMain(){}

void YangkvMain::init() {
    idx_ = 0;
    compacter_ = new Compacter();
    for (int id = 0; id < kMaxWriter; id++) {
        writer_[id] = new Writer(compacter_);
        arg_[id] = new WriterConfig(0, id, writer_[id]);
        pthread_t tid;
        pthread_create(&tid, NULL, writerRound, (void*)arg_[id]);
    }
    sleep(1);
}

Status YangkvMain::setKey(const string& key, const string& value, bool del_flag) {
	auto id = idx_++;
    int writerID = strHash(key, kSeedForWriter) % kMaxWriter;
    auto queue = writer_[writerID]->queue_;
    assert(queue != nullptr);
    MemEntry entry = MemEntry(key, value, id, del_flag);
	queue->push(entry);
    return Status::OK();
}

Status YangkvMain::delKey(const string& key) {
	Status s = setKey(key, "", true);
    return Status::OK();
}

Status YangkvMain::getValue(const string& key, string* value) {
    auto id = idx_;
    int writerID = strHash(key, kSeedForWriter) % kMaxWriter;
    Status s;
    bool result = writer_[writerID]->queue_->search(key, id, value, s);
    if (result) {
        return s;
    }
    //printf("%s %d\n", key.c_str(), __LINE__);
    result = writer_[writerID]->searchMessage(key, value, id);
    if (result) {
        return s;
    }
    assert(false);
}


void YangkvMain::stop() {
    for (int id = 0; id < kMaxWriter; id++) {
        arg_[id]->stopFLAG = true;
    }
}

