#include "utils/dbformat.h"
#include "utils/logwriter.h"
#include <mutex>
#pragma once

namespace yangkv {

class Env;
std::string LogFileName(uint64_t file_number, uint32_t writer_number);
class MessageQueue {
public:
	MessageQueue() {};
	MessageQueue(uint32_t writerID, int logID, Env* env);
    ~MessageQueue();
	void push(MemEntry entry);
	MemEntry& getFront();
    void pop();
	bool isFull();
	bool isEmpty();
	bool ready();
	void Debug();
	uint64_t maxid() {
		return max_id_;
	}
    bool search(std::string& key, uint64_t seq, std::string* value, Status* s);
private:
	static const int kQueueSize = 40;
	MemEntry queue_[kQueueSize + 5];
	uint64_t w_ptr, r_ptr;
	log::Writer* log_;
	WritableFile* logfile_;
	int logid_;
	int writerid_;
	uint64_t max_id_;
	bool ready_;
	std::mutex lock_;
	Env* env_;
};

}