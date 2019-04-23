#include "utils/dbformat.h"
#include "utils/logwriter.h"
#include <mutex>
#pragma once

namespace leveldb {

class Env;

class MessageQueue {
public:
	MessageQueue() {};
	MessageQueue(uint32_t writerid, Env* env);
    ~MessageQueue();
	void push(MemEntry entry);
	MemEntry& getFront();
    void pop();
	bool isFull();
	bool isEmpty();
	void Debug();
    bool search(std::string& key, uint64_t seq, std::string* value, Status* s);
private:
	static const int kQueueSize = 40;
	MemEntry queue_[kQueueSize + 5];
	uint64_t w_ptr, r_ptr;
	log::Writer* log_;
	WritableFile* logfile_;
	int logid_;
	int writerid_;
	std::mutex lock_;
	Env* env_;
};

}