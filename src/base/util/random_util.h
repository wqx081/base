#ifndef BASE_RANDOM_UTIL_H
#define BASE_RANDOM_UTIL_H

#include <cstdlib>
#include <stdint.h>

namespace base {

class Random;

// Writes exactly n random bytes to dest using the parameter Random generator.
// Note RandomString() does not null-terminate its strings, though '\0' could
// be written to dest with the same probability as any other byte.
void RandomString(void* dest, size_t n, Random* rng);

// Generate a 32-bit random seed from several sources, including timestamp,
// pid & tid.
uint32_t GetRandomSeed32();

} // namespace base

#endif // BASE_RANDOM_UTIL_H
