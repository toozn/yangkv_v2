#include "utils/config.h"
#include "utils/dbformat.h"
#include "skiplist.h"
#include "message_queue.h"
#pragma once

namespace yangkv {

class VersionSet;
class WriterConfig;
class Env;

class Writer{
public:
	Writer(){};
    Writer(VersionSet* set, WriterConfig* config, Env* env);
    ~Writer();
	bool getEntry(std::string& key, uint64_t seq, std::string* value, Status* s);
	void workRound();
	void mayInsertMessage();
	MessageQueue* queue_;
private:
	int id_;
	SkipList* list_;
    VersionSet* set_;
    pthread_rwlock_t rwlock_;
    Env* env_;
    WriterConfig* config_;
};

}
