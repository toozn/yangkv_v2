#include "utils/env.h"
#include "utils/hash_helper.h"
#include "utils/status.h"
#include "utils/config.h"
#include "utils/condlock.h"
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
	Writer* writer_[kMaxWriter];
    WriterConfig* writer_args_[kMaxWriter];
    Compacter* compacter_;
    CompacterConfig* compacter_args_;
    VersionSet* set_;
    pthread_cond_t compact_lock_;
    CondLock* bg_lock_;
};

}