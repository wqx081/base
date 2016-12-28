#ifndef BASE_UTIL_ENV_H_
#define BASE_UTIL_ENV_H_
#include <cstdarg>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "base/core/callback_forward.h"
#include "base/util/status.h"

namespace base {

class FileLock;
class RandomAccessFile;
class RWFile;
class SequentialFile;
class Slice;
class WritableFile;

struct RandomAccessFileOptions;
struct RWFileOptions;
struct WritableFileOptions;

// 整体使用 Bridge 模式
class Env {
 public:
  enum CreateMode {
    CREATE_IF_NON_EXISTING_TRUNCATE,
    CREATE_NON_EXISTING,
    OPEN_EXISTING
  };

  Env() {}
  virtual ~Env();

  // 单例模式
  static Env* Default();

  // 工厂模式
  virtual Status NewSequentialFile(const std::string& fname,
                                   std::unique_ptr<SequentialFile>* result) = 0;

  virtual Status NewRandomAccessFile(const std::string& fname,
                                     std::unique_ptr<RandomAccessFile>* result) = 0;
  virtual Status NewRandomAccessFile(const RandomAccessFileOptions& opts,
                                     const std::string& fname,
                                     std::unique_ptr<RandomAccessFile>* result) = 0;

  virtual Status NewWritableFile(const std::string& fname,
                                 std::unique_ptr<WritableFile>* result) = 0;
  virtual Status NewWritableFile(const WritableFileOptions& opts,
                                 const std::string& fname,
                                 std::unique_ptr<WritableFile>* result) = 0;
  virtual Status NewTempWritableFile(const WritableFileOptions& opts,
                                     const std::string& name_template,
                                     std::string* created_fname,
                                     std::unique_ptr<WritableFile>* result) = 0;

  virtual Status NewRWFile(const std::string& fname,
                           std::unique_ptr<RWFile>* result) = 0;
  virtual Status NewRWFile(const RWFileOptions& opts,
                           const std::string& fname,
                           std::unique_ptr<RWFile>* result) = 0;
  virtual Status NewTempRWFile(const RWFileOptions& opts,
                               const std::string& name_template,
                               std::string* created_fname,
                               std::unique_ptr<RWFile>* result) = 0;

  // 通用文件/目录操作 
  virtual bool FileExists(const std::string& fname) = 0;
  virtual Status GetChildren(const std::string& dir,
                             std::vector<std::string>* result) = 0;
  virtual Status DeleteFile(const std::string& fname) = 0;
  virtual Status CreateDir(const std::string& dirname) = 0;
  virtual Status DeleteDir(const std::string& dirname) = 0;
  virtual Status SyncDir(const std::string& dirname) = 0;
  virtual Status DeleteRecursively(const std::string& dirname) = 0;
  virtual Status GetFileSize(const std::string& fname, uint64_t* file_size) = 0;
  virtual Status GetFileSizeOnDisk(const std::string& fname, uint64_t* file_size) = 0;
  virtual Status GetFileSizeOnDiskRecursively(const std::string& root, uint64_t* bytes_used) = 0;
  virtual Status GetFileModifiedTime(const std::string& fname, int64_t* timestamp) = 0;
  virtual Status GetBlockSize(const std::string& fname, uint64_t* block_size) = 0;
  virtual Status GetBytesFree(const std::string& path, int64_t* bytes_free) = 0;
  virtual Status RenameFile(const std::string& src,
                            const std::string& target) = 0;

  virtual Status LockFile(const std::string& fname, FileLock** lock) = 0;
  virtual Status UnlockFile(FileLock* lock) = 0;

  virtual Status GetTestDirectory(std::string* path) = 0;
  
  virtual Status GetExecutablePath(std::string* path) = 0;
  virtual Status IsDirectory(const std::string& path, bool* is_dir) = 0;

  enum FileType {
    DIRECTORY_TYPE,
    FILE_TYPE,
  };

  typedef core::Callback<Status(FileType,
                                const std::string&,
                                const std::string&)> WalkCallback;
  enum DirectoryOrder {
    PRE_ORDER,
    POST_ORDER,
  };

  virtual Status Walk(const std::string& root,
                      DirectoryOrder order,
                      const WalkCallback& cb) = 0;
  virtual Status Glob(const std::string& path_pattern,
                      std::vector<std::string>* paths) = 0;
  virtual Status Canonicalize(const std::string& path, std::string* result) = 0;
  virtual Status GetTotalRAMBytes(int64_t* ram) = 0;
  virtual int64_t GetOpenFileLimit() = 0;
  virtual void IncreaseOpenFileLimit() = 0;
  virtual Status IsOnExtFilesystem(const std::string& path, bool* result) = 0;
  virtual std::string GetKernelRelease() = 0;
  
  // Misc
  virtual uint64_t NowMicros() = 0;
  virtual void SleepForMicroseconds(int micros) = 0;
  virtual uint64_t gettid() = 0;
  
  
 private:
  DISALLOW_COPY_AND_ASSIGN(Env);
};

class SequentialFile {
 public:
  SequentialFile() {}
  virtual ~SequentialFile();

  virtual Status Read(size_t n, Slice* result, uint8_t* scratch) = 0;
  virtual Status Skip(uint64_t n) = 0;
  virtual const std::string& filename() const = 0;
};

class RandomAccessFile {
 public:
  RandomAccessFile() {}
  virtual ~RandomAccessFile();

  virtual Status Read(uint64_t offset, size_t n, Slice* result, uint8_t* scratch) const = 0;
  virtual Status Size(uint64_t* size) const = 0;
  virtual const std::string& filename() const = 0;
  virtual size_t memory_footprint() const = 0;
};

struct WritableFileOptions {
  bool sync_on_close;
  Env::CreateMode mode;

  WritableFileOptions()
    : sync_on_close(false),
      mode(Env::CREATE_IF_NON_EXISTING_TRUNCATE) {
  }
};

struct RandomAccessFileOptions {
  RandomAccessFileOptions() {}
};

class WritableFile {
 public:
  enum FlushMode {
    FLUSH_SYNC,
    FLUSH_ASYNC
  };
  
  WritableFile() {}
  virtual ~WritableFile();

  virtual Status Append(const Slice& data) = 0;
  virtual Status AppendVector(const std::vector<Slice>& data_vector) = 0;
  virtual Status PreAllocate(uint64_t size) = 0;
  virtual Status Close() = 0;
  virtual Status Flush(FlushMode mode) = 0;
  virtual Status Sync() = 0;
  virtual uint64_t Size() const = 0;
  virtual const std::string& filename() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(WritableFile);
};

struct RWFileOptions {
  bool sync_on_close;
  Env::CreateMode mode;
  
  RWFileOptions()
    : sync_on_close(false),
      mode(Env::CREATE_IF_NON_EXISTING_TRUNCATE) {
  }
};

class RWFile {
 public:
  enum FlushMode {
    FLUSH_SYNC,
    FLUSH_ASYNC,
  };

  RWFile() {}
  virtual ~RWFile();

  virtual Status Read(uint64_t offset, size_t length,
                      Slice* result, uint8_t* scratch) const = 0;
  virtual Status Write(uint64_t, const Slice& data) = 0;
  virtual Status PreAllocate(uint64_t offset, size_t length) = 0;
  virtual Status Truncate(uint64_t length) = 0;
  virtual Status PunchHole(uint64_t offset, size_t length) = 0;
  virtual Status Flush(FlushMode mode, uint64_t offset, size_t length) = 0;
  virtual Status Sync() = 0;
  virtual Status Close() = 0;
  virtual Status Size(uint64_t* size) const = 0;
  virtual const std::string& filename() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(RWFile);
};

class FileLock {
 public:
  FileLock() {}
  virtual ~FileLock();
 private:
  DISALLOW_COPY_AND_ASSIGN(FileLock);
};

extern Status WriteStringToFile(Env* env, const Slice& data,
                                const std::string& fname);
extern Status WriteStringToFileSync(Env* env, const Slice& data,
                                    const std::string& fname);
extern Status ReadFileToString(Env* env, const std::string& fname,
                               faststring* result);

} // namespace base
#endif // BASE_UTIL_ENV_H_
