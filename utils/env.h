#ifndef STORAGE_LEVELDB_UTIL_ENV_H
#define STORAGE_LEVELDB_UTIL_ENV_H
#include <string>
#pragma once

namespace leveldb {
class Env {
public:
	Env();
};
static const int kSeedForWriter = 37;
static const int kQueueSizeToWriter = 32;
static const int kQueueSizeToCompacter = 16;
static const int kMaxWriter = 4;
static const int kMaxLevel = 7;
static const int kMaxActiveListSize = 1024 * 128; //128KB
static const int kSlowDownWrite = 8; // If more than 8 SSTable in lv0, slow down the write process once
static const int kMaxTableNumInLevel0 = 12;// cease the write process, waiting for compact
static const std::string sNotFound = "NotFound";
static const std::string sError = "Error";
static const int kDeleted = 1;
	
}

#endif
