#include "dbformat.h"
#pragma once
using namespace std;

class MessageQueue {
public:
	MessageQueue();
    ~MessageQueue();
	void push(MemEntry entry);
	MemEntry getFront();
    void pop();
	bool isFull();
	bool isEmpty();
    bool search(std::string& key, uint64_t seq, std::string* value, Status* s);
private:
	static const int kQueueSize = 20;
	MemEntry queue_[kQueueSize];
	unsigned long long w_ptr, r_ptr;
	mutex lock_;
};
