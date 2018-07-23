#ifndef LOGGER_H
#define LOGGER_H

#include <algorithm>
#include<stdarg.h>    /* va_list, va_start, va_arg, va_end */

#include "file_stream.h"
#include "Logfmt.h"

struct SprintfException: public std::exception {
  SprintfException(const char* msg) : msg_(msg) {}

  const char* what() const throw() { return msg_; }
  const char* msg_;
};

struct LogfmtException : public std::exception {
  LogfmtException(const char* msg) : msg_(msg) {}

  const char* what() const throw() { return msg_; }
  const char* msg_;
};

namespace kin_logfmt {

  enum LEVEL {
    FATAL_  =0,
    ERROR_  =1,
    WARN_   =2,
    WARNING_=2,
    INFO_,
    DEBUG_,

    NUM_LEVELS,
  };

  static const std::string LEVEL_STR[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG",
  };

  static_assert(sizeof(LEVEL_STR)/sizeof(std::string) == NUM_LEVELS,
                "sizes don't match");

  typedef std::string logfmt_key_t;

  // Reserved key fields
  enum class LOGFMT_KEY {
    module,
    timestamp,
    msg,
    tag,
    request_id,
    client,

    NUM_KEYS,
  };

  static const logfmt_key_t LOGFMT_KEY_STR[] = {
    "module",
    "timestamp",
    "msg",
    "tag",
    "request_id",
    "client",
  };

  static_assert(sizeof(LOGFMT_KEY_STR)/sizeof(logfmt_key_t) == (long unsigned int)(LOGFMT_KEY::NUM_KEYS),
                "sizes don't match");

  static logfmt_key_t to_string(LOGFMT_KEY key) {
    return LOGFMT_KEY_STR[(unsigned int)key];
  }

  /*
    Comparison function(s) that Logger should use when printing logfmt key order
    Currently, we simply follow the order of the LOGFMT_KEY enum
  */
  static bool fn_default_key_order(const logfmt_key_t lhs, const logfmt_key_t rhs) {
    auto lhs_order = std::find(LOGFMT_KEY_STR, LOGFMT_KEY_STR+(unsigned int)LOGFMT_KEY::NUM_KEYS, lhs);
    auto rhs_order = std::find(LOGFMT_KEY_STR, LOGFMT_KEY_STR+(unsigned int)LOGFMT_KEY::NUM_KEYS, rhs);
    return lhs_order < rhs_order;
  }

  const int MAX_MSG_LENGTH = 500;


  class Logger {
    public:

    Logger(LEVEL log_level, FileStream *stream, const std::string& module);

    void FATAL(const std::string& msg, ...);
    void ERROR(const std::string& msg, ...);
    void WARN(const std::string& msg, ...);
    void WARNING(const std::string& msg, ...);
    void INFO(const std::string& msg, ...);
    void DEBUG(const std::string& msg, ...);

    // The following functions may throw LogfmtException if the format does not comply
    static std::string compile_logfmt_content(const std::string& msg, va_list& args);
    std::string compile_logfmt_metadata();

    // Override sprintf
    // Throws SprintfException if the args do not comply
    static int sprintf(char *buffer, const char* format, const int arg_count, va_list& args);

    private:

    void write(const std::string& msg, va_list& args);

    LEVEL log_level_;
    FileStream *stream_;
    const std::string& module_name_;
  };
}; // namespace kin_logfmt

#endif // LOGGER_H
