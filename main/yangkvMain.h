#include "utils/env.h"
#include "utils/hash_helper.h"
#include "utils/status.h"
#pragma once

class Writer;
class Compacter;
class WriterConfig;
class VersionSet;
class YangkvMain {
public:
	YangkvMain();
	~YangkvMain();
    void init();
	Status setKey(const string&, const string&, bool flag = 0);
	Status delKey(const string&);
	Status getValue(const string&, string&);
	Writer* getWriter(int);
    void stop();

private:
	friend class Compacter;
	friend class Writer;
	friend class VersionSet;
	unsigned long long idx_;
	Env* env_;
    static const int kMaxWriter = 4;
    static const int kSeedForWriter = 37;
	Writer* writer_[kMaxWriter];
	Compacter* compacter_;
    WriterConfig* arg_[kMaxWriter];
    VersionSet* set_;
};

void* workerRound(void*);
