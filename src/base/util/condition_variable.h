#ifndef BASE_UTIL_CONDITION_VARIABLE_H_
#define BASE_UTIL_CONDITION_VARIABLE_H_

#include <memory>

#include "base/util/monotime.h"
#include "base/util/mutex.h"

namespace base {
namespace util {

class ConditionVariable {
 public:
  explicit ConditionVariable(Mutex* user_lock);

  ~ConditionVariable();

  void Wait() const;
  bool TimedWait(const MonoDelta& max_time) const;
  void Broadcast();
  void Signal();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
  DISALLOW_COPY_AND_ASSIGN(ConditionVariable);
};

} // namespace util
} // namespace base
#endif // BASE_UTIL_CONDITION_VARIABLE_H_
