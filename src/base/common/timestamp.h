#ifndef KUDU_COMMON_TIMESTAMP_H_
#define KUDU_COMMON_TIMESTAMP_H_

#include <cinttypes>
#include <iosfwd>
#include <string>

namespace base {

class faststring;
class Slice;

// A transaction timestamp generated by a Clock.
class Timestamp {
 public:
  typedef uint64_t val_type;

  Timestamp() : v(kInvalidTimestamp.v) {}

  explicit Timestamp(uint64_t val) : v(val) {}

  // Decode a timestamp from the given input slice.
  // Mutates the slice to point after the decoded timestamp.
  // Returns true upon success.
  bool DecodeFrom(Slice* input);

  // Encode the timestamp to the given buffer.
  void EncodeTo(faststring* dst) const;

  // Returns -1 if this < other.
  // Returns 1 if this > other.
  // Returns 0 if this == other.
  int CompareTo(const Timestamp& other) const;

  std::string ToString() const;

  // Returns this Timestamp as an uint64_t
  uint64_t ToUint64() const;

  // Sets this Timestamp from 'value'
  void FromUint64(uint64_t value);

  val_type value() const { return v; }

  // An initial transaction timestamp, higher than min so that we can have
  // a Timestamp guaranteed to be lower than all generated timestamps.
  static const Timestamp kInitialTimestamp;

  // An invalid transaction timestamp -- Timestamp types initialize to this variable.
  static const Timestamp kInvalidTimestamp;

  // The maximum timestamp.
  static const Timestamp kMax;

  // The minimum timestamp.
  static const Timestamp kMin;

 private:
  friend bool operator==(const Timestamp& lhs, const Timestamp& rhs);
  friend bool operator!=(const Timestamp& lhs, const Timestamp& rhs);
  friend bool operator<(const Timestamp& lhs, const Timestamp& rhs);
  friend bool operator<=(const Timestamp& lhs, const Timestamp& rhs);
  friend bool operator>(const Timestamp& lhs, const Timestamp& rhs);
  friend bool operator>=(const Timestamp& lhs, const Timestamp& rhs);

  val_type v;
};

std::ostream& operator<<(std::ostream& o, const Timestamp& timestamp);

inline int Timestamp::CompareTo(const Timestamp& other) const {
  if (v < other.v) {
    return -1;
  } else if (v > other.v) {
    return 1;
  }
  return 0;
}

inline bool operator==(const Timestamp& lhs, const Timestamp& rhs) {
  return lhs.v == rhs.v;
}

inline bool operator!=(const Timestamp& lhs, const Timestamp& rhs) {
  return !(lhs.v == rhs.v);
}

inline bool operator<(const Timestamp& lhs, const Timestamp& rhs) {
  return lhs.v < rhs.v;
}

inline bool operator>(const Timestamp& lhs, const Timestamp& rhs) {
  return rhs < lhs;
}

inline bool operator<=(const Timestamp& lhs, const Timestamp& rhs) {
  return !(lhs > rhs);
}

inline bool operator>=(const Timestamp& lhs, const Timestamp& rhs) {
  return !(lhs < rhs);
}

} // namespace kudu

#endif /* KUDU_COMMON_TIMESTAMP_H_ */