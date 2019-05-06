
#include "logwriter.h"

#include <stdint.h>
#include "utils/env.h"
#include "utils/dbformat.h"
#include "main/versionedit.h"

namespace yangkv {
namespace log {


Writer::Writer(WritableFile* dest)
    : dest_(dest){

}

Writer::~Writer() {
}

Status Writer::AddRecord(MemEntry& entry) {
  Slice slice;
  EncodeMemEntry(entry, slice);
  size_t sz = slice.size();
  Status s = dest_->AppendSize(sz);
  s = dest_->Append(slice);
  assert(s.ok());
  s = dest_->Flush();
  assert(s.ok());
  return Status::OK();
}

Status Writer::AddVersionEdit(VersionEdit* v) {

}

}  // namespace log
}  // namespace yangkv
