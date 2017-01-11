#ifndef KUDU_UTIL_NVM_CACHE_H_
#define KUDU_UTIL_NVM_CACHE_H_

#include <string>

namespace base {
class Cache;

// Create a cache in persistent memory with the given capacity.
Cache* NewLRUNvmCache(size_t capacity, const std::string& id);

}  // namespace base

#endif
