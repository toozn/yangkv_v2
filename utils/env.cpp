#include "env.h"

namespace leveldb {
Status Env::NewRandomAccessFile(const std::string& filename,
                             RandomAccessFile** result) {
    *result = nullptr;
    int fd = ::open(filename.c_str(), O_RDONLY);
    if (fd < 0) {
      return Status::Error();
    }

    *result = new RandomAccessFile(filename, fd);
    return Status::OK();
    /*
    uint64_t file_size;
    Status status = GetFileSize(filename, &file_size);
    if (status.ok()) {
      void* mmap_base = ::mmap(nullptr, file_size, PROT_READ,
                               MAP_SHARED, fd, 0);
      if (mmap_base != MAP_FAILED) {
        *result = new PosixMmapReadableFile(
            filename, reinterpret_cast<char*>(mmap_base), file_size,
            &mmap_limiter_);
      } else {
        status = Status::Error();
      }
    }
    ::close(fd);
    if (!status.ok()) {
      mmap_limiter_.Release();
    }
    return status;
    */
  }

Status Env::NewWritableFile(const std::string& filename,
                         WritableFile** result) {
    int fd = ::open(filename.c_str(), O_TRUNC | O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
      *result = nullptr;
      return Status::Error();
    }
    *result = new WritableFile(filename, fd);
    return Status::OK();

}  RandomAccessFile::RandomAccessFile(std::string filename, int fd):
        fd_(fd),
        filename_(std::move(filename)) {
  }

  RandomAccessFile::~RandomAccessFile() {
  }

  Status RandomAccessFile::Read(uint64_t offset, size_t n, Slice* result,
              char* scratch) const {
    int fd = fd_;

    assert(fd != -1);
    Status status;
    ssize_t read_size = ::pread(fd, scratch, n, static_cast<off_t>(offset));
    *result = Slice(scratch, (read_size < 0) ? 0 : read_size);
    if (read_size < 0) {
      // An error: return a non-ok status.
      status = Status::Error();
    }
    return status;
  }

  WritableFile::WritableFile(std::string filename, int fd)
      : pos_(0), fd_(fd), is_manifest_(IsManifest(filename)),
        filename_(std::move(filename)), dirname_(Dirname(filename_)) {}

  WritableFile::~WritableFile() {
    if (fd_ >= 0) {
      // Ignoring any potential errors
      Close();
    }
  }

  Status WritableFile::Append(const Slice& data) {
    size_t write_size = data.size();
    const char* write_data = data.data();

    // Fit as much as possible into buffer.
    size_t copy_size = std::min(write_size, kWritableFileBufferSize - pos_);
    std::memcpy(buf_ + pos_, write_data, copy_size);
    write_data += copy_size;
    write_size -= copy_size;
    pos_ += copy_size;
    if (write_size == 0) {
      return Status::OK();
    }

    // Can't fit in buffer, so need to do at least one write.
    Status status = FlushBuffer();
    if (!status.ok()) {
      return status;
    }

    // Small writes go to buffer, large writes are written directly.
    if (write_size < kWritableFileBufferSize) {
      std::memcpy(buf_, write_data, write_size);
      pos_ = write_size;
      return Status::OK();
    }
    return WriteUnbuffered(write_data, write_size);
  }

  Status WritableFile::Close() {
    Status status = FlushBuffer();
    const int close_result = ::close(fd_);
    if (close_result < 0 && status.ok()) {
      status = Status::Error();
    }
    fd_ = -1;
    return status;
  }

  Status WritableFile::Flush() {
    return FlushBuffer();
  }

  Status WritableFile::Sync() {
    // Ensure new files referred to by the manifest are in the filesystem.
    //
    // This needs to happen before the manifest file is flushed to disk, to
    // avoid crashing in a state where the manifest refers to files that are not
    // yet on disk.
    Status status = SyncDirIfManifest();
    if (!status.ok()) {
      return status;
    }

    status = FlushBuffer();
    if (status.ok() && ::fdatasync(fd_) != 0) {
      status = Status::Error();
    }
    return status;
  }
  Status WritableFile::FlushBuffer() {
    Status status = WriteUnbuffered(buf_, pos_);
    pos_ = 0;
    return status;
  }

  Status WritableFile::WriteUnbuffered(const char* data, size_t size) {
    while (size > 0) {
      ssize_t write_result = ::write(fd_, data, size);
      if (write_result < 0) {
        if (errno == EINTR) {
          continue;  // Retry
        }
        return Status::Error();
      }
      data += write_result;
      size -= write_result;
    }
    return Status::OK();
  }

  Status WritableFile::SyncDirIfManifest() {
    Status status;
    if (!is_manifest_) {
      return status;
    }

    int fd = ::open(dirname_.c_str(), O_RDONLY);
    if (fd < 0) {
      status = Status::Error();
    } else {
      if (::fsync(fd) < 0) {
        status = Status::Error();
      }
      ::close(fd);
    }
    return status;
  }

  // Returns the directory name in a path pointing to a file.
  //
  // Returns "." if the path does not contain any directory separator.
  std::string WritableFile::Dirname(const std::string& filename) {
    std::string::size_type separator_pos = filename.rfind('/');
    if (separator_pos == std::string::npos) {
      return std::string(".");
    }
    // The filename component should not contain a path separator. If it does,
    // the splitting was done incorrectly.
    assert(filename.find('/', separator_pos + 1) == std::string::npos);

    return filename.substr(0, separator_pos);
  }

  // Extracts the file name from a path pointing to a file.
  //
  // The returned Slice points to |filename|'s data buffer, so it is only valid
  // while |filename| is alive and unchanged.
  Slice WritableFile::Basename(const std::string& filename) {
    std::string::size_type separator_pos = filename.rfind('/');
    if (separator_pos == std::string::npos) {
      return Slice(filename);
    }
    // The filename component should not contain a path separator. If it does,
    // the splitting was done incorrectly.
    assert(filename.find('/', separator_pos + 1) == std::string::npos);

    return Slice(filename.data() + separator_pos + 1,
                 filename.length() - separator_pos - 1);
  }

  // True if the given file is a manifest file.
  bool WritableFile::IsManifest(const std::string& filename) {
  	return false;
    //return Basename(filename).starts_with("MANIFEST");
  }


}