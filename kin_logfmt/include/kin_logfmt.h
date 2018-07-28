#ifndef KIN_LOGFMT_H
#define KIN_LOGFMT_H

#include <vector>
#include <iostream>

#include "logger.h"

namespace kin_logfmt {

  class KinLogfmt {
    public:

    KinLogfmt(LEVEL log_level = INFO_, FileStream *stream = NULL);
    ~KinLogfmt();

    template <class ...Args>
    Logger* new_logger(const std::string& module, const Args&... args) {
      return new Logger(log_level_, stream_, module, args...);
    }

    private:

    LEVEL log_level_;
    FileStream *stream_;
  };

}; // namespace kin_logfmt

#endif // KIN_LOGFMT_H
