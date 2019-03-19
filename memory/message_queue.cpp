#include "message_queue.h"
#include "utils/env.h"
#include <unistd.h>
#include <iostream>
using namespace std;
namespace leveldb {

MessageQueue::MessageQueue() {
	w_ptr = 1;
	r_ptr = 1;
}

MessageQueue::~MessageQueue() {
}

void MessageQueue::push(MemEntry entry) {
	std::lock_guard<std::mutex> lock(lock_);
    while(isFull()) usleep(5);
	queue_[w_ptr % kQueueSize] = entry;
	w_ptr++;
}

void MessageQueue::pop() {
	assert(w_ptr > r_ptr);
	r_ptr++;
}

MemEntry& MessageQueue::getFront() {
    assert(w_ptr > r_ptr);
    return queue_[(r_ptr % kQueueSize)];
}

bool MessageQueue::isFull() {
	return (w_ptr - r_ptr == kQueueSize);
}

bool MessageQueue::isEmpty() {
	return (w_ptr == r_ptr);
}

bool MessageQueue::search(std::string& key, uint64_t seq, std::string* value, Status* s) {
    uint64_t begin = w_ptr - 1;
    uint64_t end = r_ptr;
    for (uint64_t i = begin; i >= end; i--) {
        MemEntry entry = queue_[(i % kQueueSize)];
        if (key == entry.key.ToString() && seq <= entry.seq_num) {
            if (entry.value_type == kDeleted) {
                *s = Status::NotFound();
            }
            else {
                *s = Status::OK();
                value->assign(entry.value.data(), entry.value.size());
            }
            return true;
        }
    }
    return false;
}

void MessageQueue::Debug() {
    uint64_t begin = w_ptr;
    uint64_t end = r_ptr;
    assert(end <= kQueueSize + begin);
    for (uint64_t i = begin; i >= end; i--) {
        MemEntry entry = queue_[(i % kQueueSize)];
        entry.Debug();
    }
    puts("-----------------------------");
}

}
