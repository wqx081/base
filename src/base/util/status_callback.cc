#include "base/util/status_callback.h"

#include "base/util/status.h"

using std::string;

namespace base {


void DoNothingStatusCB(const Status& /* status */) {}

void CrashIfNotOkStatusCB(const string& message, const Status& status) {
  if (PREDICT_FALSE(!status.ok())) {
    LOG(FATAL) << message << ": " << status.ToString();
  }
}

Status DoNothingStatusClosure() { return Status::OK(); }

} // end namespace mprmpr
