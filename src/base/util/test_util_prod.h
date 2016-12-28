// Test-related utility methods that can be called from non-test
// code. This module is part of the 'util' module and is built into
// all binaries, not just tests, whereas 'test_util.cc' is linked
// only into test binaries.

#pragma once

namespace base {

// Return true if the current binary is a gtest. More specifically,
// returns true if the 'test_util.cc' module has been linked in
// (either dynamically or statically) to the running process.
bool IsGTest();

} // namespace base
