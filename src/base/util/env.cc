#include "base/util/env.h"

namespace base {

Env::~Env() {}

SequentialFile::~SequentialFile() {}

RandomAccessFile::~RandomAccessFile() {}

WritableFile::~WritableFile() {}

RWFile::~RWFile() {}

FileLock::~FileLock() {}

namespace {

Status DoWriteStringToFile(Env* env, const Slice& data, const std::string& fname,
                           bool should_sync) {
  std::unique_ptr<WritableFile> file;
  Status s = env->NewWritableFile(fname, &file);
  if (!s.ok()) {
    return s;
  }
  s = file->Append(data);
  if (s.ok() && should_sync) {
    s = file->Sync();
  }
  if (s.ok()) {
    s = file->Close();
  }
  file.reset();
  if (!s.ok()) {
    WARN_NOT_OK(env->DeleteFile(fname), "Failed to delete partially-written file " + fname);
  }
  return s;
}

} // namespace

Status WriteStringToFile(Env* env,
                         const Slice& data,
                         const std::string& fname) {
  return DoWriteStringToFile(env, data, fname, false);
}

Status WriteStringToFileSync(Env* env,
                             const Slice& data,
                             const std::string& fname) {
  return DoWriteStringToFile(env, data, fname, true);
}

Status ReadFileToString(Env* env, const std::string& fname, faststring* result) {
  result->clear();
  std::unique_ptr<SequentialFile> file;
  Status s = env->NewSequentialFile(fname, &file);
  if (!s.ok()) {
    return s;
  }
  static const int kBufferSize = 8192;
  std::unique_ptr<uint8_t[]> scratch(new uint8_t[kBufferSize]);
  while (true) {
    Slice fragment;
    s = file->Read(kBufferSize, &fragment, scratch.get());
    if (!s.ok()) {
      break;
    }
    result->append(fragment.data(), fragment.size());
    if (fragment.empty()) {
      break;
    }
  }
  return s;
}

} // namespace base
