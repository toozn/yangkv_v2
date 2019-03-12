#include "utils/env.h"
#include "utils/hash_helper.h"
#include "utils/status.h"
#include "utils/config.h"
#include <atomic>
#pragma once

namespace leveldb {

class Writer;
class Compacter;
class WriterConfig;
class VersionSet;
class YangkvMain {
public:
	YangkvMain();
	~YangkvMain();
    void init();
	Status setKey(std::string& key, std::string& value, bool flag = 0);
	Status delKey(std::string& key);
	Status getValue(std::string&, std::string*);
	Writer* getWriter(int);
    void stop();

private:
	friend class Compacter;
	friend class Writer;
	friend class VersionSet;
	std::atomic <uint64_t> idx_;
	Env* env_;
    static const int kMaxWriter = 4;
    static const int kSeedForWriter = 37;
	Writer* writer_[kMaxWriter];
	Compacter* compacter_;
    WriterConfig* arg_[kMaxWriter];
    VersionSet* set_;
};

void* workerRound(void*);

}