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
  for (auto logger : loggers_) {
    delete logger;
  }
  delete stream_;
}

Logger* KinLogfmt::newLogger(const std::string& module) {
  auto logger = new Logger(log_level_, stream_, module);
  loggers_.push_back(logger);
  return logger;
}
