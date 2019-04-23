// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "logwriter.h"

#include <stdint.h>
#include "utils/env.h"
#include "utils/dbformat.h"

namespace leveldb {
namespace log {


Writer::Writer(WritableFile* dest)
    : dest_(dest){

}

Writer::~Writer() {
}

Status Writer::AddRecord(MemEntry& entry) {
  Slice slice;
  entry.Debug();
  EncodeMemEntry(entry, slice);
  Status s = dest_->Append(slice);
  assert(s.ok());
  s = dest_->Flush();
  assert(s.ok());
  return Status::OK();
}
}  // namespace log
}  // namespace leveldb
