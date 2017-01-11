#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "base/common/timestamp.h"
#include "base/common/clock.h"
#include "base/core/ref_counted.h"
#include "base/util/locks.h"
#include "base/util/status.h"

namespace base {
namespace consensus {
class ReplicateMsg;

class TimeManager : public core::RefCountedThreadSafe<TimeManager> {
 public:

  TimeManager(scoped_refptr<Clock> clock,  Timestamp initial_safe_time);

  void SetLeaderMode();

  void SetNonLeaderMode();

  Status AssignTimestamp(ReplicateMsg* message);

  Status MessageReceivedFromLeader(const ReplicateMsg& message);

  void AdvanceSafeTimeWithMessage(const ReplicateMsg& message);

  void AdvanceSafeTime(Timestamp safe_time);

  Status WaitUntilSafe(Timestamp timestamp, const MonoTime& deadline);

  Timestamp GetSafeTime();
 private:
  FRIEND_TEST(TimeManagerTest, TestTimeManagerNonLeaderMode);
  FRIEND_TEST(TimeManagerTest, TestTimeManagerLeaderMode);

  bool HasAdvancedSafeTimeRecentlyUnlocked(std::string* error_message);

  bool IsSafeTimeLaggingUnlocked(Timestamp timestamp, std::string* error_message);

  void MakeWaiterTimeoutMessageUnlocked(Timestamp timestamp, std::string* error_message);

  static ExternalConsistencyMode GetMessageConsistencyMode(const ReplicateMsg& message);

  // The mode of this TimeManager.
  enum Mode {
    LEADER,
    NON_LEADER
  };

  // State for waiters.
  struct WaitingState {
    Timestamp timestamp;
    CountDownLatch* latch;
  };

  bool IsTimestampSafeUnlocked(Timestamp timestamp);

  void AdvanceSafeTimeAndWakeUpWaitersUnlocked(Timestamp safe_time);

  Timestamp GetSerialTimestamp();

  Timestamp GetSerialTimestampPlusMaxError();

  Timestamp GetSafeTimeUnlocked();

  mutable simple_spinlock lock_;

  mutable std::vector<WaitingState*> waiters_;

  Timestamp last_serial_ts_assigned_;

  Timestamp last_safe_ts_;

  MonoTime last_advanced_safe_time_;

  Mode mode_;

  const scoped_refptr<Clock> clock_;
  const std::string local_peer_uuid_;
};

} // namespace consensus
} // namespace base
