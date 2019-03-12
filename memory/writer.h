#include "utils/config.h"
#include "skiplist.h"
#include "message_queue.h"
#pragma once

namespace leveldb {

class Compacter;
class Writer{
public:
    Writer(){}
    Writer(Compacter*);
    ~Writer();
	bool getEntry(std::string& key, uint64_t seq, std::string* value, Status* s);
	void mayInsertMessage();
	MessageQueue* queue_;
private:
    const static int kMaxListSize = 102400;
    Compacter* compacter_;
	SkipList* list_;
    pthread_rwlock_t rwlock_;
};

}
