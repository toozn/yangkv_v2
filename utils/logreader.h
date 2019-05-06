#include "env.h"
#include "dbformat.h"
#include "slice.h"
#include "status.h"
#pragma once
namespace yangkv {
class RandomAccessFile;  
namespace log {
class Reader {
 public:
  // Create a writer that will append data to "*dest".
  // "*dest" must have initial length "dest_length".
  // "*dest" must remain live while this Reader is in use.
  Reader(RandomAccessFile* dest);
  ~Reader();

  Status ReadRecord(MemEntry& entry);
  bool end();

 private:
  RandomAccessFile* dest_;
  size_t offset_;
  char* buf_;
  size_t size_;
  // No copying allowed
  Reader(const Reader&);
  void operator=(const Reader&);
};

}  // namespace log
}  // namespace yangkv
