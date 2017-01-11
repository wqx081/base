#ifndef BASE_POOL_SERVICE_POOL_H_
#define BASE_POOL_SERVICE_POOL_H_

#include <string>
#include <vector>

#include "base/core/macros.h"
#include "base/core/gscoped_ptr.h"
#include "base/core/ref_counted.h"
#include "base/rpc/rpc_service.h"
#include "base/rpc/service_queue.h"
#include "base/util/mutex.h"
#include "base/util/thread.h"
#include "base/util/status.h"

namespace base {

class Counter;
class Histogram;
class MetricEntity;
class Socket;

namespace rpc {

class Messenger;
class ServiceIf;

// A pool of threads that handle new incoming RPC calls.
// Also includes a queue that calls get pushed onto for handling by the pool.
class ServicePool : public RpcService {
 public:
  ServicePool(gscoped_ptr<ServiceIf> service,
              const scoped_refptr<MetricEntity>& metric_entity,
              size_t service_queue_length);
  virtual ~ServicePool();

  // Start up the thread pool.
  virtual Status Init(int num_threads);

  // Shut down the queue and the thread pool.
  virtual void Shutdown();

  RpcMethodInfo* LookupMethod(const RemoteMethod& method) override;

  virtual Status QueueInboundCall(gscoped_ptr<InboundCall> call) OVERRIDE;

  const Counter* RpcsTimedOutInQueueMetricForTests() const {
    return rpcs_timed_out_in_queue_.get();
  }

  const Histogram* IncomingQueueTimeMetricForTests() const {
    return incoming_queue_time_.get();
  }

  const Counter* RpcsQueueOverflowMetric() const {
    return rpcs_queue_overflow_.get();
  }

  const std::string service_name() const;

 private:
  void RunThread();
  void RejectTooBusy(InboundCall* c);

  gscoped_ptr<ServiceIf> service_;
  std::vector<scoped_refptr<base::Thread> > threads_;
  LifoServiceQueue service_queue_;
  scoped_refptr<Histogram> incoming_queue_time_;
  scoped_refptr<Counter> rpcs_timed_out_in_queue_;
  scoped_refptr<Counter> rpcs_queue_overflow_;

  mutable Mutex shutdown_lock_;
  bool closing_;

  DISALLOW_COPY_AND_ASSIGN(ServicePool);
};

} // namespace rpc
} // namespace base
#endif // BASE_RPC_SERVICE_POOL_H_
