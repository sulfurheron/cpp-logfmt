#ifndef LOGGER_H
#define LOGGER_H

#include <algorithm>
#include <stdarg.h>    /* va_list, va_start, va_arg, va_end */

#include "file_stream.h"
#include "logfmt_kv.h"

struct SprintfException: public std::exception {
  SprintfException(const char* msg) : msg_(msg) {}

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

  static std::string to_string(LEVEL level) {
    return LEVEL_STR[(unsigned int)level];
  }

  /*
    Comparison function(s) that Logger should use when printing logfmt key order
    Currently, we simply follow the order of the LOGFMT_KEY enum
  */
  static bool fn_default_key_order(const std::string lhs, const std::string rhs) {
    auto lhs_order = std::find(LOGFMT_KEY_STR, LOGFMT_KEY_STR+(unsigned int)LOGFMT_KEY::NUM_KEYS, lhs);
    auto rhs_order = std::find(LOGFMT_KEY_STR, LOGFMT_KEY_STR+(unsigned int)LOGFMT_KEY::NUM_KEYS, rhs);
    return lhs_order < rhs_order;
  }

  const int MAX_MSG_LENGTH = 500;


  class Logger {
    public:

    template <class ...Args>
    Logger(LEVEL log_level, FileStream *stream, const std::string& module, const Args&... args);

    template <class ...Args>
    void FATAL(const std::string& msg, const Args&... args);
    template <class ...Args>
    void ERROR(const std::string& msg, const Args&... args);
    template <class ...Args>
    void WARN(const std::string& msg, const Args&... args);
    template <class ...Args>
    void WARNING(const std::string& msg, const Args&... args);
    template <class ...Args>
    void INFO(const std::string& msg, const Args&... args);
    template <class ...Args>
    void DEBUG(const std::string& msg, const Args&... args);

    // The following functions may throw LogfmtException if the format does not comply
    template <class ...Args>
    std::string compile_logfmt_content(const std::string& msg, const Args&... args);

    std::string compile_logfmt_metadata(LEVEL level);

    // Override sprintf
    // Throws SprintfException if the args do not comply
    template <class ...Args>
    static int sprintf(char *buffer, const char* format, const int arg_count, const Args&... args);

    private:

    template <class ...Args>
    void write(LEVEL level, const std::string& msg, const Args&... args);

    template <class ...Args>
    static std::vector<logfmt_kv_t> construct_kv_pairs(int arg_start, int arg_end, const Args&... args);

    LEVEL log_level_;
    FileStream *stream_;
    const std::string& module_name_;

    LogFmtMessage context_msg_;
  };
}; // namespace kin_logfmt

#include "logger_impl.h"

#endif // LOGGER_H
