#ifndef BASE_UTIL_SCOPED_CLEANUP_H_
#define BASE_UTIL_SCOPED_CLEANUP_H_

#include <utility>

namespace base {

// A scoped object which runs a cleanup function when going out of scope. Can
// be used for scoped resource cleanup.
template<typename F>
class ScopedCleanup {
 public:
  explicit ScopedCleanup(F f)
      : cancelled_(false),
        f_(std::move(f)) {
  }
  ~ScopedCleanup() {
    if (!cancelled_) {
      f_();
    }
  }
  void cancel() { cancelled_ = true; }

 private:
  bool cancelled_;
  F f_;
};

// Creates a new scoped cleanup instance with the provided function.
template<typename F>
ScopedCleanup<F> MakeScopedCleanup(F f) {
  return ScopedCleanup<F>(f);
}

} // namespace base
#endif
