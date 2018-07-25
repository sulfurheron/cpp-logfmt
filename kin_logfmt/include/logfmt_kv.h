#ifndef LOGFMT_VALS_H
#define LOGFMT_VALS_H

#include <boost/variant.hpp>

#include "Logfmt.h"


struct LogfmtException : public std::exception {
  LogfmtException(const char* msg) : msg_(msg) {}

  const char* what() const throw() { return msg_; }
  const char* msg_;
};

namespace kin_logfmt {

  /* The following definitions are used for the logfmt key type */

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

  static const std::string LOGFMT_KEY_STR[] = {
    "module",
    "timestamp",
    "msg",
    "tag",
    "request_id",
    "client",
  };

  static_assert(sizeof(LOGFMT_KEY_STR)/sizeof(std::string) == (long unsigned int)(LOGFMT_KEY::NUM_KEYS),
                "sizes don't match");

  static std::string to_string(LOGFMT_KEY key) {
    return LOGFMT_KEY_STR[(unsigned int)key];
  }

  typedef boost::variant<LOGFMT_KEY, char*, std::string> logfmt_key_t;

  /*
    N.B. There is a bug converting char* types within boost::variant and other overloaded operators:
         char* and bool will both be deduced as the bool type. To get around this, we use a force_bool
         struct to explicity define bools

    https://stackoverflow.com/questions/13268608/boostvariant-why-is-const-char-converted-to-bool
  */
  struct force_bool {
    explicit force_bool(bool val)
      : val_(val) {}

    bool val_;
  };

  // This struct asserts that a correct type is used for boost::variant logfmt_key_t
  // The overloaded operator() must support every type enumerated in logfmt_key_and_value_union_t
  struct fn_assert_key_type : public boost::static_visitor<> {
    public:

    void operator()(const LOGFMT_KEY l) {
      key_ = to_string(l);
    }

    void operator()(const char* c) {
      key_ = std::string(c);
    }

    void operator()(const std::string s) {
      key_ = s;
    }

    void operator()(const int) {
      throw LogfmtException("int was passed as type logfmt_key_t");
    }

    void operator()(const double) {
      throw LogfmtException("double was passed as type logfmt_key_t");
    }

    void operator()(const force_bool) {
      throw LogfmtException("bool was passed as type logfmt_key_t");
    }

    std::string key_;
  };

  /* The following definitions are used for the logfmt value type */

  typedef boost::variant<char*, std::string, int, double, force_bool> logfmt_val_t;

  // This struct asserts that a correct type is used for boost::variant logfmt_val_t
  // The overloaded operator() must support every type enumerated in logfmt_key_and_value_union_t
  struct fn_assert_variable_content_type : public boost::static_visitor<> {
    public:

    fn_assert_variable_content_type(logfmt_val_t &val)
      : val_(val) {}

    void operator()(const LOGFMT_KEY) {
      throw LogfmtException("LOGFMT_KEY was passed as type logfmt_val_t");
    }

    void operator()(const char* c) {
      val_ = c;
    }

    void operator()(const std::string s) {
      val_ = s;
    }

    void operator()(const int i) {
      val_ = i;
    }

    void operator()(const double f) {
      val_ = f;
    }

    void operator()(const force_bool b) {
      val_ = b;
    }

    logfmt_val_t val_;
  };

  struct fn_insert_variable_content_type : public boost::static_visitor<> {
    public:

    fn_insert_variable_content_type(LogFmtMessage &msg, const std::string &key)
      : msg_(msg),
        key_(key) {}

    void operator()(const force_bool b) {
      msg_.insert(key_, b.val_);
    }

    template <typename T>
    void operator()(T t) {
      msg_.insert(key_, t);
    }

    private:

    LogFmtMessage &msg_;
    std::string key_;
  };


  // For ease in unpacking the variadic template, configure this union type to support both key and
  // value-supported types
  typedef boost::variant<LOGFMT_KEY,char*,std::string,int,double,force_bool> logfmt_key_and_value_union_t;

  typedef std::pair<std::string,logfmt_val_t> logfmt_kv_t;

}; // namespace kin_logfmt

#endif // LOGFMT_VALS_H
