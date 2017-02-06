#ifndef BASE_CORE_INLINED_VECTOR_H_
#define BASE_CORE_INLINED_VECTOR_H_

#include <cstddef>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

#include "base/core/manual_constructor.h"
#include "base/core/sysinfo.h"
#include "base/core/port.h"

#include <initializer_list>

namespace core {
// namespace gtl {

// 避免动态内存分配
template <typename T, int N>
class InlinedVector {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef pointer iterator;
  typedef const_pointer const_iterator;

  InlinedVector();
  explicit InlinedVector(size_t n);
  InlinedVector(size_t n, const value_type& element)
  template <typename InputIterator>
  InlinedVector(InputIterator range_start,
                InputIterator range_end,
                typename std::enable_if<!std::is_integral<InputIterator>::value>::type* = nullptr);

  InlinedVector(std::initializer_list<value_type> init);

  // Copy-Ctor
  InlinedVector(const InlinedVector& v);
  ~InlinedVector();

  // Copy-Assignment-Op
  InlinedVector& operator=(const InlinedVector& v);

  size_t size() const;
  bool empty() const;


 private:
  static const size_t kSizeUnaligned = N * sizeof(T) + 1;
  static const size_t kSize = ((kSizeUnaligned + 15) / 16) * 16;

  static const unsigned int kSentinel = 255;
  static const size_t kFit1 = (kSize - 1) / sizeof(T);
  static const size_t kFit = (kFit1 >= kSentinel) ? (kSentinel - 1) : kFit1;

  // data
  union {
    unsigned char data[kSize];
    //TODO(wqx):
    T* unused_aligner;
  } u_;

};

// } // namespace gtl
} // namespace core
#endif // BASE_CORE_INLINED_VECTOR_H_
