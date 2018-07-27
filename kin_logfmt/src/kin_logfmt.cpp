#include "kin_logfmt.h"

using namespace kin_logfmt;

KinLogfmt::KinLogfmt(LEVEL log_level, FileStream *stream)
  : log_level_(log_level),
    stream_(stream) {

  if (stream_ == NULL) {
    stream_ = new StdErr();
  }
}

KinLogfmt::~KinLogfmt() {
  delete stream_;
}
