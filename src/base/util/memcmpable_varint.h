#ifndef KUDU_UTIL_MEMCMPABLE_VARINT_H
#define KUDU_UTIL_MEMCMPABLE_VARINT_H

#include "base/util/faststring.h"
#include "base/util/slice.h"


namespace base {

void PutMemcmpableVarint64(faststring *dst, uint64_t value);

// Standard Get... routines parse a value from the beginning of a Slice
// and advance the slice past the parsed value.
bool GetMemcmpableVarint64(Slice *input, uint64_t *value);

} // namespace base

#endif
