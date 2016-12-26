// ManualConstructor statically-allocates space in which to store some
// object, but does not initialize it.  You can then call the constructor
// and destructor for the object yourself as you see fit.  This is useful
// for memory management optimizations, where you want to initialize and
// destroy an object multiple times but only allocate it once.
//
// (When I say ManualConstructor statically allocates space, I mean that
// the ManualConstructor object itself is forced to be the right size.)
//
// For example usage, check out util/gtl/small_map.h.

#ifndef UTIL_GTL_MANUAL_CONSTRUCTOR_H_
#define UTIL_GTL_MANUAL_CONSTRUCTOR_H_

#include <stddef.h>

#include "base/core/port.h"

namespace core {

namespace util {
namespace gtl {
namespace internal {

//
// Provides a char array with the exact same alignment as another type. The
// first parameter must be a complete type, the second parameter is how many
// of that type to provide space for.
//
//   UTIL_GTL_ALIGNED_CHAR_ARRAY(struct stat, 16) storage_;
//
// Because MSVC and older GCCs require that the argument to their alignment
// construct to be a literal constant integer, we use a template instantiated
// at all the possible powers of two.
#ifndef SWIG
template<int alignment, int size> struct AlignType { };
template<int size> struct AlignType<0, size> { typedef char result[size]; };
#if defined(_MSC_VER)
#define UTIL_GTL_ALIGN_ATTRIBUTE(X) __declspec(align(X))
#define UTIL_GTL_ALIGN_OF(T) __alignof(T)
#elif defined(__GNUC__) || defined(__APPLE__) || defined(__INTEL_COMPILER) \
  || defined(__nacl__)
#define UTIL_GTL_ALIGN_ATTRIBUTE(X) __attribute__((aligned(X)))
#define UTIL_GTL_ALIGN_OF(T) __alignof__(T)
#endif

#if defined(UTIL_GTL_ALIGN_ATTRIBUTE)

#define UTIL_GTL_ALIGNTYPE_TEMPLATE(X) \
  template<int size> struct AlignType<X, size> { \
    typedef UTIL_GTL_ALIGN_ATTRIBUTE(X) char result[size]; \
  }

UTIL_GTL_ALIGNTYPE_TEMPLATE(1);
UTIL_GTL_ALIGNTYPE_TEMPLATE(2);
UTIL_GTL_ALIGNTYPE_TEMPLATE(4);
UTIL_GTL_ALIGNTYPE_TEMPLATE(8);
UTIL_GTL_ALIGNTYPE_TEMPLATE(16);
UTIL_GTL_ALIGNTYPE_TEMPLATE(32);
UTIL_GTL_ALIGNTYPE_TEMPLATE(64);
UTIL_GTL_ALIGNTYPE_TEMPLATE(128);
UTIL_GTL_ALIGNTYPE_TEMPLATE(256);
UTIL_GTL_ALIGNTYPE_TEMPLATE(512);
UTIL_GTL_ALIGNTYPE_TEMPLATE(1024);
UTIL_GTL_ALIGNTYPE_TEMPLATE(2048);
UTIL_GTL_ALIGNTYPE_TEMPLATE(4096);
UTIL_GTL_ALIGNTYPE_TEMPLATE(8192);
// Any larger and MSVC++ will complain.

#define UTIL_GTL_ALIGNED_CHAR_ARRAY(T, Size) \
  typename util::gtl::internal::AlignType<UTIL_GTL_ALIGN_OF(T), \
                                          sizeof(T) * Size>::result

#undef UTIL_GTL_ALIGNTYPE_TEMPLATE
#undef UTIL_GTL_ALIGN_ATTRIBUTE

#else  // defined(UTIL_GTL_ALIGN_ATTRIBUTE)
#error "You must define UTIL_GTL_ALIGNED_CHAR_ARRAY for your compiler."
#endif  // defined(UTIL_GTL_ALIGN_ATTRIBUTE)

#else  // !SWIG

// SWIG can't represent alignment and doesn't care about alignment on data
// members (it works fine without it).
template<typename Size>
struct AlignType { typedef char result[Size]; };
#define UTIL_GTL_ALIGNED_CHAR_ARRAY(T, Size) \
    util::gtl::internal::AlignType<Size * sizeof(T)>::result

// Enough to parse with SWIG, will never be used by running code.
#define UTIL_GTL_ALIGN_OF(Type) 16

#endif  // !SWIG

}  // namespace internal
}  // namespace gtl
}  // namespace util

template <typename Type>
class ManualConstructor {
 public:
  // No constructor or destructor because one of the most useful uses of
  // this class is as part of a union, and members of a union cannot have
  // constructors or destructors.  And, anyway, the whole point of this
  // class is to bypass these.

  // Support users creating arrays of ManualConstructor<>s.  This ensures that
  // the array itself has the correct alignment.
  static void* operator new[](size_t size) {
    return aligned_malloc(size, UTIL_GTL_ALIGN_OF(Type));
  }
  static void operator delete[](void* mem) {
    aligned_free(mem);
  }

  inline Type* get() {
    return reinterpret_cast<Type*>(space_);
  }
  inline const Type* get() const  {
    return reinterpret_cast<const Type*>(space_);
  }

  inline Type* operator->() { return get(); }
  inline const Type* operator->() const { return get(); }

  inline Type& operator*() { return *get(); }
  inline const Type& operator*() const { return *get(); }

  inline void Init() { new (space_) Type; }

  template <typename... Ts>
  inline void Init(Ts&&... args) {                 // NOLINT
    new (space_) Type(std::forward<Ts>(args)...);  // NOLINT
  }

  inline void Destroy() {
    get()->~Type();
  }

 private:
  UTIL_GTL_ALIGNED_CHAR_ARRAY(Type, 1) space_;
};

#undef UTIL_GTL_ALIGNED_CHAR_ARRAY
#undef UTIL_GTL_ALIGN_OF

}

#endif  // UTIL_GTL_MANUAL_CONSTRUCTOR_H_
