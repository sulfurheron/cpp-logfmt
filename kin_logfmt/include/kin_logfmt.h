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

    Logger* newLogger(const std::string& module);

    private:

    LEVEL log_level_;
    FileStream *stream_;

    std::vector<Logger*> loggers_;
  };

}; // namespace kin_logfmt

#endif // KIN_LOGFMT_H
