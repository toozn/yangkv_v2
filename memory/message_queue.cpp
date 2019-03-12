#include "message_queue.h"
#include <unistd.h>

MessageQueue::MessageQueue() {
	w_ptr = 1;
	r_ptr = 1;
}

MessageQueue::~MessageQueue() {
    unsigned long long begin = r_ptr;
    unsigned long long end = w_ptr - 1;
    for (unsigned long long i = begin; i <= end; i++) {
        int pos = i % kQueueSize;
        delete queue_[pos];
    }
}

void MessageQueue::push(MemEntry msg) {
	lock_guard<mutex> lock(lock_);
    //msg->Debug();
	while(w_ptr - r_ptr >= kQueueSize) usleep(5);
	queue_[w_ptr % kQueueSize] = msg;
	w_ptr++;
}

void MessageQueue::pop() {
	assert(w_ptr > r_ptr);
	r_ptr++;
}

Message* MessageQueue::getFront() {
    assert(w_ptr > r_ptr);
    return queue_[r_ptr % kQueueSize];
}

bool MessageQueue::isFull() {
	return (w_ptr - r_ptr == kQueueSize);
}

bool MessageQueue::isEmpty() {
	return (w_ptr == r_ptr);
}

bool MessageQueue::search(std::string& key, uint64_t seq, std::string* value, Status* s) {
    unsigned long long begin = w_ptr - 1;
    unsigned long long end = r_ptr;
    for (unsigned long long i = begin; i >= end; i--) {
        MemEntry entry = queue_[i % kQueueSize];
        if (key == entry.key.ToString() && seq <= entry.seq_num) {
            if (entry.value_type == kNotFound) {
                s = Status::NotFound();
            }
            else {
                s = Status::OK();
                value->assign(entry.value.data(), entry.value.size());
            }
            return true;
        }
    }
    return false;
}
