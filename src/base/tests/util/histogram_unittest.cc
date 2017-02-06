#include <gtest/gtest.h>
#include <atomic>

#include "base/util/status.h"
#include "base/util/test_util.h"

namespace base {

class AbstractHistogramIterator;
class Status;
class RecordedValuesIterator;


class Histogram {
 public:
  Histogram(uint64_t highest_trackable_value, int num_sig_digits);
  explicit Histogram(const Histogram& other);

  static bool IsValidHighestTrackableValue(uint64_t highest_trackable_value);
  static bool IsValidNumSignificantDigits(int num_sig_digits);

  // Record
  void Increment(int value);
  void Increment(int64_t value, int64_t count);

 private:
  friend class AbstractHistogramIterator;

  std::atomic<uint64_t> total_count_;  
  std::atomic<uint64_t> total_sum_;
  std::atomic<uint64_t> min_value_;
  std::atomic<uint64_t> max_value_;
  
};

//
// static const int kSigDigits = 2;

} // namespace base
