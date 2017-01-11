#ifndef KUDU_SERVER_CLOCK_H_
#define KUDU_SERVER_CLOCK_H_

#include <string>

#include "base/common/common.pb.h"
#include "base/common/timestamp.h"

#include "base/core/ref_counted.h"
#include "base/util/monotime.h"
#include "base/util/status.h"

namespace base {

class faststring;
class MetricEntity;
class Slice;
class Status;

class Clock : public core::RefCountedThreadSafe<Clock> {
 public:

  virtual Status Init() = 0;
  virtual Timestamp Now() = 0;
  virtual Timestamp NowLatest() = 0;

  virtual Status GetGlobalLatest(Timestamp* t) {
    return Status::NotSupported("clock does not support global properties");
  }

  virtual bool SupportsExternalConsistencyMode(ExternalConsistencyMode mode) = 0;

  virtual bool HasPhysicalComponent() const {
    return false;
  }

  virtual MonoDelta GetPhysicalComponentDifference(Timestamp /*lhs*/, Timestamp /*rhs*/) const {
    LOG(FATAL) << "Clock's timestamps don't have a physical component.";
  }

  virtual Status Update(const Timestamp& to_update) = 0;

  virtual Status WaitUntilAfter(const Timestamp& then,
                                const MonoTime& deadline) = 0;

  virtual Status WaitUntilAfterLocally(const Timestamp& then,
                                       const MonoTime& deadline) = 0;

  virtual bool IsAfter(Timestamp t) = 0;

  virtual void RegisterMetrics(const scoped_refptr<MetricEntity>& metric_entity) = 0;

  virtual std::string Stringify(Timestamp timestamp) = 0;

  virtual ~Clock() {}
};

} // namespace base 
#endif /* KUDU_SERVER_CLOCK_H_ */
