
#ifndef STORAGE_yangkv_DB_LOG_WRITER_H_
#define STORAGE_yangkv_DB_LOG_WRITER_H_

#include <stdint.h>
#include "dbformat.h"
#include "slice.h"
#include "status.h"

namespace yangkv {

class WritableFile;
class VersionEdit; 

namespace log {

class Writer {
 public:
  // Create a writer that will append data to "*dest".
  // "*dest" must be initially empty.
  // "*dest" must remain live while this Writer is in use.
  explicit Writer(WritableFile* dest);

  // Create a writer that will append data to "*dest".
  // "*dest" must have initial length "dest_length".
  // "*dest" must remain live while this Writer is in use.
  Writer(WritableFile* dest, uint64_t dest_length);
  ~Writer();

  Status AddRecord(MemEntry& entry);
  Status AddVersionEdit(VersionEdit* edit);

 private:
  WritableFile* dest_;
  
  // No copying allowed
  Writer(const Writer&);
  void operator=(const Writer&);
};

}  // namespace log
}  // namespace yangkv

#endif  // STORAGE_yangkv_DB_LOG_WRITER_H_
