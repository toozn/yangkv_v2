#include "utils/dbformat.h"
#include <mutex>
#pragma once

namespace leveldb {

class MessageQueue {
public:
	MessageQueue();
    ~MessageQueue();
	void push(MemEntry entry);
	MemEntry getFront();
    void pop();
	bool isFull();
	bool isEmpty();
	void debug();
    bool search(std::string& key, uint64_t seq, std::string* value, Status* s);
private:
	static const int kQueueSize = 40;
	MemEntry queue_[kQueueSize + 5];
	uint64_t w_ptr, r_ptr;
	std::mutex lock_;
};

}