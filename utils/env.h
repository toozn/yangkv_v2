#ifndef STORAGE_LEVELDB_UTIL_ENV_H
#define STORAGE_LEVELDB_UTIL_ENV_H
#include <string>
#pragma once

namespace leveldb {
class Env {
public:
	Env();
};

const int kQueueSizeToWriter = 32;
const int kQueueSizeToCompacter = 16;
const int kMaxWriter = 4;
const int kMaxLevel = 7;
const int kMaxActiveListSize = 1024 * 128; //128KB
const int kSlowDownWrite = 8; // If more than 8 SSTable in lv0, slow down the write process once
const int kMaxTableNumInLevel0 = 12;// cease the write process, waiting for compact
const std::string sNotFound = "NotFound";
const std::string sError = "Error";
const int kDeleted = 0;
const int kExist = 1;
std::string NullStr = "";
	
}

#endif
