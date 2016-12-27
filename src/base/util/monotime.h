//@file monotime.h
//
// MonoDelta 表示时间区间
// MonoTime  表示特定时间点
//

#ifndef BASE_UTIL_MONOTIME_H_
#define BASE_UTIL_MONOTIME_H_

#include <stdint.h>
#include <string>

struct timeval;
struct timespec;

namespace base {
namespace util {

class MonoTime;

class MonoDelta {
  friend class MonoTime; 

 public:
  
  // Use for Ctor.
  static MonoDelta FromSeconds(double seconds);
  static MonoDelta FromMilliseconds(int64_t ms);
  static MonoDelta FromMicroseconds(int64_t us);
  static MonoDelta FromNanoseconds(int64_t ns);

  // TODO(wqx):
  // FromTimeVal
  // FromTimeSpec
  // FromString

  MonoDelta();
  bool Initialized() const;
  bool LessThan(const MonoDelta& x) const;
  bool MoreThan(const MonoDelta& x) const;
  bool Equals(const MonoDelta& x) const;

  double ToSeconds() const;
  int64_t ToMilliseconds() const;
  int64_t ToMicroseconds() const;
  int64_t ToNanoseconds() const;
  
  void ToTimeVal(struct timeval* tv) const;
  void ToTimeSpec(struct timespec* ts) const;
  std::string ToString() const;
  
  static void NanosToTimeSpec(int64_t nanos, struct timespec* ts);

 private:
  static const int64_t kUninitialized;

  explicit MonoDelta(int64_t delta);
  int64_t nano_delta_;
};

class MonoTime {
  friend class MonoDelta;

 public:
  static const int64_t kNanosecondsPerSecond = 1000000000L;
  static const int64_t kNanosecondsPerMillisecond = 1000000L;
  static const int64_t kNanosecondsPerMicrosecond = 1000L;

  static const int64_t kMicrosecondsPerSecond = 1000000L;

  static MonoTime Now();
  static MonoTime Max();
  static MonoTime Min();
  static const MonoTime& Earliest(const MonoTime& a, const MonoTime& b);
  
  MonoTime();
  bool Initialized() const;
  MonoDelta GetDeltaSince(const MonoTime& x) const;
  void AddDelta(const MonoDelta& d);

  // 本时间点是否比 x 早
  bool ComesBefore(const MonoTime& x) const;
  
  std::string ToString() const;
  bool Equals(const MonoTime& x) const;

  MonoTime& operator+=(const MonoDelta& d);
  MonoTime& operator-=(const MonoDelta& d);

 private:
  explicit MonoTime(const struct timespec& ts);
  explicit MonoTime(int64_t nanos);
  double ToSeconds() const;

  int64_t nanos_;
};

// For MonoDelta
void SleepFor(const MonoDelta& delta);
bool operator==(const MonoDelta& x, const MonoDelta& y);
bool operator!=(const MonoDelta& x, const MonoDelta& y);
bool operator<(const MonoDelta& x, const MonoDelta& y);
bool operator<=(const MonoDelta& x, const MonoDelta& y);
bool operator>(const MonoDelta& x, const MonoDelta& y);
bool operator>=(const MonoDelta& x, const MonoDelta& y);

// For MonoTime
bool operator==(const MonoTime& x, const MonoTime& y);
bool operator!=(const MonoTime& x, const MonoTime& y);
bool operator<(const MonoTime& x, const MonoTime& y);
bool operator<=(const MonoTime& x, const MonoTime& y);
bool operator>(const MonoTime& x, const MonoTime& y);
bool operator>=(const MonoTime& x, const MonoTime& y);

MonoTime operator+(const MonoTime& t, const MonoDelta& d);
MonoTime operator-(const MonoTime& t, const MonoDelta& d);
MonoDelta operator-(const MonoTime& t, const MonoTime& begin);

} // namespace util
} // namespace base
#endif // BASE_UTIL_MONOTIME_H_
