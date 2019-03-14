#include "utils/config.h"
#include "utils/dbformat.h"
#include "skiplist.h"
#include "message_queue.h"
#pragma once

namespace leveldb {

class VersionSet;
class Writer{
public:
	Writer(){};
    Writer(VersionSet* set, int id);
    ~Writer();
	bool getEntry(std::string& key, uint64_t seq, std::string* value, Status* s);
	void mayInsertMessage();
	MessageQueue queue_;
private:
	int id_;
	SkipList* list_;
    VersionSet* set_;
    pthread_rwlock_t rwlock_;
};

}
