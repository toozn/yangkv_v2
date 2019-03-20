
#ifndef _ENV_H
#define _ENV_H
#include <string>
#include "status.h"
#include "slice.h"
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#pragma once

namespace leveldb {
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
static const uint64_t kTableMagicNumber = 0xdb4775248b80fb57ull;
static const size_t kBlockTrailerSize = 5;
enum CompressionType {
  kNoCompression     = 0x0,
  kSnappyCompression = 0x1
};

class RandomAccessFile {
 public:
  // The new instance takes ownership of |fd|. |fd_limiter| must outlive this
  // instance, and will be used to determine if .

  RandomAccessFile(std::string filename, int fd);

  ~RandomAccessFile();

  Status Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const;

 private:
  const int fd_;  // -1 if has_permanent_fd_ is false.
  //Limiter* const fd_limiter_;
  const std::string filename_;
};

class WritableFile{
 public:
  WritableFile(std::string filename, int fd);

  ~WritableFile();

  Status Append(const Slice& data);

  Status Close() ;

  Status Flush() ;

  Status Sync();

 private:
  Status FlushBuffer();

  Status WriteUnbuffered(const char* data, size_t size) ;

  Status SyncDirIfManifest();

  // Returns the directory name in a path pointing to a file.
  //
  // Returns "." if the path does not contain any directory separator.
  static std::string Dirname(const std::string& filename);
  // Extracts the file name from a path pointing to a file.
  //
  // The returned Slice points to |filename|'s data buffer, so it is only valid
  // while |filename| is alive and unchanged.
  static Slice Basename(const std::string& filename);

  // True if the given file is a manifest file.
  static bool IsManifest(const std::string& filename) ;

  // buf_[0, pos_ - 1] contains data to be written to fd_.
  char buf_[kWritableFileBufferSize];
  size_t pos_;
  int fd_;

  const bool is_manifest_;  // True if the file's name starts with MANIFEST.
  const std::string filename_;
  const std::string dirname_;  // The directory of filename_.
};

class Env {
public:
  Env() {};
  ~Env() {};
  Status NewWritableFile(const std::string& filename, WritableFile** result);
  Status NewRandomAccessFile(const std::string& filename, RandomAccessFile** result);
};

}

#endif