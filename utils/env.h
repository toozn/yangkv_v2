#include <string>
#include "status.h"
#include "slice.h"
#pragma once

namespace leveldb {
class PosixWritableFile;
class Env {
public:
	Env() {};
	~Env() {};
	Status NewWritableFile(const std::string& filename, PosixWritableFile** result);
};
static const int kSeedForWriter = 37;
static const int kQueueSizeToWriter = 32;
static const int kQueueSizeToCompacter = 16;
static const int kMaxWriter = 4;
static const int kMaxLevel = 7;
static const int kMaxFrozenList = 10;
static const int kMaxActiveListSize = 10; //128KB
static const int kSlowDownWrite = 8; // If more than 8 SSTable in lv0, slow down the write process once
static const int kMaxTableNumInLevel0 = 12;// cease the write process, waiting for compact
static const size_t kWritableFileBufferSize = 65536;
static const int kDeleted = 1;
	
}

