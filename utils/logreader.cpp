
#include "logreader.h"
#include <stdint.h>
#include "utils/env.h"
#include "utils/dbformat.h"

namespace yangkv {
namespace log {


Reader::Reader(RandomAccessFile* dest)
    : dest_(dest){
      offset_ = 0;
      buf_ = new char[kMaxRecordSize];
      size_ = dest_->FileSize();
      printf("file size: %d\n", int(size_));
}

Reader::~Reader() {
  delete buf_;
}

bool Reader::end() {
  return offset_ + 4 >= size_;
}

Status Reader::ReadRecord(MemEntry& entry) {
  Slice slice;
  size_t record_size;
  Status s = dest_->ReadSize(offset_, &record_size, buf_);
  printf("data size: %d\n", record_size);
  s = dest_->Read(offset_ + sizeof(size_t), record_size, &slice, buf_);
  DecodeMemEntry(slice, entry);
  offset_ += sizeof(size_t) + record_size;
  return Status::OK();
}
}  // namespace log
}  // namespace yangkv
