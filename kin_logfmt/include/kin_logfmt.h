#ifndef KIN_LOGFMT_H
#define KIN_LOGFMT_H

#include <vector>
#include <iostream>

#include "logger.h"

namespace kin_logfmt {

  class KinLogfmt {
    public:

    KinLogfmt(LEVEL log_level, FileStream *stream = NULL);
    ~KinLogfmt();

    template <class ...Args>
    Logger* newLogger(const std::string& module, const Args&... args) {
      auto logger = new Logger(log_level_, stream_, module, args...);
      loggers_.push_back(logger);
      return logger;
    }

    private:

    LEVEL log_level_;
    FileStream *stream_;

    std::vector<Logger*> loggers_;
  };

}; // namespace kin_logfmt

#endif // KIN_LOGFMT_H
