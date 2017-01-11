#ifndef BASE_RAFT_RESETTABLE_HEARTBEATER_H_
#define BASE_RAFT_RESETTABLE_HEARTBEATER_H_
#include <functional>
#include <string>

#include "base/core/gscoped_ptr.h"
#include "base/core/macros.h"
#include "base/util/monotime.h"

namespace base {

class Status;
class ResettableHeartbeaterThread;

typedef std::function<Status()> HeartbeatFunction;

class ResettableHeartbeater {
 public:
  ResettableHeartbeater(const std::string& name,
                        MonoDelta period,
                        HeartbeatFunction function);
  Status Start();
  Status Stop();

  void Reset();

  ~ResettableHeartbeater();

 private:
  gscoped_ptr<ResettableHeartbeaterThread> thread_;
  DISALLOW_COPY_AND_ASSIGN(ResettableHeartbeater);
};

} // namespace base
#endif // BASE_RAFT_RESETTABLE_HEARTBEATER_H_
