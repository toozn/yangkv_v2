#include "message_queue.h"
#include "utils/logreader.h"
#include "utils/dbformat.h"
#include "utils/env.h"
#include <unistd.h>
#include <iostream>
#include <math.h>
#pragma once
namespace yangkv {

std::string LogFileName(uint64_t file_number, uint32_t writer_number) {
    return "yang_log_writer_" + std::to_string(writer_number)
     + "_" + std::to_string(file_number);
}

MessageQueue::MessageQueue(uint32_t writerID, int logID, Env* env) {
    ready_ = false;
	w_ptr = 1;
	r_ptr = 1;
    logid_ = logID;
    env_ = env;
    logfile_ = nullptr;
    max_id_ = 0;
    writerid_ = writerID;
    Status s = env_->NewWritableFile(
        LogFileName(logid_, writerid_), &logfile_);
    assert(logfile_ != nullptr);
    log_ = new log::Writer(logfile_);
    for (int i = 1; i < logid_; i++) {
        RandomAccessFile* oldlog = nullptr;
        Status s = env_->NewRandomAccessFile(
        LogFileName(i, writerid_), &oldlog);
        assert(oldlog != nullptr);
        auto reader = new log::Reader(oldlog);
        MemEntry entry;
        while(!reader->end()) {
            s = reader->ReadRecord(entry);
            max_id_ = std::max(max_id_, entry.seq_num);
            push(entry);
        }
    }
    ready_ = true;
}

bool MessageQueue::ready() {
    return ready_;
}

MessageQueue::~MessageQueue() {
}

void MessageQueue::push(MemEntry entry) {
	std::lock_guard<std::mutex> lock(lock_);
    if(ready_) Status s = log_->AddRecord(entry);
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
