// Base test class, with various utility functions.
#ifndef BASE_TEST_UTIL_H
#define BASE_TEST_UTIL_H

#include <functional>
#include <gtest/gtest.h>
#include <string>

#include "base/core/gscoped_ptr.h"
#include "base/util/env.h"
#include "base/util/monotime.h"
#include "base/util/test_macros.h"

namespace base {

extern const char* kInvalidPath;

class MprTest : public ::testing::Test {
 public:
  MprTest();

  virtual ~MprTest();

  virtual void SetUp() OVERRIDE;

  // Tests assume that they run with no outside-provided kerberos credentials, and if the
  // user happened to have some credentials available they might fail due to being already
  // kinitted to a different realm, etc. This function overrides the relevant environment
  // variables so that we don't pick up the user's credentials.
  static void OverrideKrb5Environment();

 protected:
  // Returns absolute path based on a unit test-specific work directory, given
  // a relative path. Useful for writing test files that should be deleted after
  // the test ends.
  std::string GetTestPath(const std::string& relative_path);

  Env* env_;
  google::FlagSaver flag_saver_;  // Reset flags on every test.
  std::string test_dir_;
};

// Returns true if slow tests are runtime-enabled.
bool AllowSlowTests();

// Override the given gflag to the new value, only in the case that
// slow tests are enabled and the user hasn't otherwise overridden
// it on the command line.
// Example usage:
//
// OverrideFlagForSlowTests(
//     "client_inserts_per_thread",
//     strings::Substitute("$0", FLAGS_client_inserts_per_thread * 100));
//
void OverrideFlagForSlowTests(const std::string& flag_name,
                              const std::string& new_value);

// Call srand() with a random seed based on the current time, reporting
// that seed to the logs. The time-based seed may be overridden by passing
// --test_random_seed= from the CLI in order to reproduce a failed randomized
// test. Returns the seed.
int SeedRandom();

// Return a per-test directory in which to store test data. Guaranteed to
// return the same directory every time for a given unit test.
//
// May only be called from within a gtest unit test. Prefer MprTest::test_dir_
// if a MprTest instance is available.
std::string GetTestDataDirectory();

// Wait until 'f()' succeeds without adding any GTest 'fatal failures'.
// For example:
//
//   AssertEventually([]() {
//     ASSERT_GT(ReadValueOfMetric(), 10);
//   });
//
// The function is run in a loop with exponential backoff, capped at once
// a second.
void AssertEventually(const std::function<void(void)>& f,
                      const MonoDelta& timeout = MonoDelta::FromSeconds(30));

// Count the number of open file descriptors in use by this process.
int CountOpenFds(Env* env);

} // namespace base
#endif
