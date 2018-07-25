#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#include "logfmt_kv.h"

using namespace kin_logfmt;

template <class ...Args>
void Logger::FATAL(const std::string& msg, const Args&... args) {
  if (FATAL_ <= log_level_) {
    write(FATAL_, msg, args...);
  }
}

template <class ...Args>
void Logger::ERROR(const std::string& msg, const Args&... args) {
  if (ERROR_ <= log_level_) {
    write(ERROR_, msg, args...);
  }
}

template <class ...Args>
void Logger::WARN(const std::string& msg, const Args&... args) {
  if (WARN_ <= log_level_) {
    write(WARN_, msg, args...);
  }
}

template <class ...Args>
void Logger::WARNING(const std::string& msg, const Args&... args) {
  if (WARNING_ <= log_level_) {
    write(WARNING_, msg, args...);
  }
}

template <class ...Args>
void Logger::INFO(const std::string& msg, const Args&... args) {
  if (INFO_ <= log_level_) {
    write(INFO_, msg, args...);
  }
}

template <class ...Args>
void Logger::DEBUG(const std::string& msg, const Args&... args) {
  if (DEBUG_ <= log_level_) {
    write(DEBUG_, msg, args...);
  }
}

template <class ...Args>
void Logger::write(LEVEL level, const std::string& msg, const Args&... args) {
  std::string metadata = compile_logfmt_metadata(level);
  std::string content = compile_logfmt_content(msg, args...);

  auto out = metadata + " " + content;
  stream_->write(out);
}

template <class ...Args>
std::string Logger::compile_logfmt_content(const std::string& msg, const Args&... args) {
  LogFmtMessage content_msg(&fn_default_key_order);
  std::vector<logfmt_key_and_value_union_t> args_vec = {args...};

  char msg_buf[MAX_MSG_LENGTH];
  int arg_count = args_vec.size();
  int format_arg_count;
  try {
    format_arg_count = sprintf(msg_buf, msg.c_str(), arg_count, args...);
  } catch (SprintfException &e) {
    std::cerr << "Error formatting msg: " << e.what() << std::endl;
  }

  content_msg.insert(to_string(LOGFMT_KEY::msg), msg_buf);

  // Check that args can be split into key-value pairs
  if (format_arg_count && (arg_count - format_arg_count) % 2 != 0) {
    throw LogfmtException("Key-value pairs are not even");
  }

  std::vector<logfmt_kv_t> kv_pairs = construct_kv_pairs(format_arg_count,
                                                         arg_count,
                                                         args...);

  for (auto it = kv_pairs.begin(); it != kv_pairs.end(); it++) {
    fn_insert_variable_content_type insert_variable_content(content_msg, it->first);
    boost::apply_visitor(insert_variable_content, it->second);
  }

  return content_msg.stringify();
}

template <class ...Args>
std::vector<logfmt_kv_t> Logger::construct_kv_pairs(int arg_start, int arg_end, const Args&... args) {
  std::vector<logfmt_kv_t> kv_pairs;
  std::vector<logfmt_key_and_value_union_t> args_vec = {args...};

  for (int i = arg_start; i < arg_end; i+=2) {
    logfmt_key_t key;
    fn_assert_key_type assert_key_type;
    boost::apply_visitor(assert_key_type, args_vec[i]);

    logfmt_val_t val;
    fn_assert_variable_content_type assert_variable_content(val);
    boost::apply_visitor(assert_variable_content, args_vec[i+1]);

    logfmt_kv_t kv_pair(assert_key_type.key_, assert_variable_content.val_);
    kv_pairs.push_back(kv_pair);
  }
  return kv_pairs;
}

/*
  Sprintf (overloaded) performs some basic tests on the formatting arguments
  to try and throw a descriptive error. It also returns the number of arguments
  it expectedly formatted (based on the number of format descriptors %), rather than
  the number of characters it formatted as sprintf does.
*/
template <class ...Args>
int Logger::sprintf(char *buffer, const char *format, const int arg_count, const Args&... args) {

  // Scan the number of expected arguments
  int format_arg_count = (*format == '%') ? 1 : 0;
  for (const char *ch = format; *ch != '\0'; ) {
    if (*ch++ != '%' && *ch == '%') {
      format_arg_count++;
    }
  }

  //int num_chars_formatted =
  ::sprintf(buffer, format, args...);

  if (arg_count < format_arg_count) {
    throw SprintfException("Not enough arguments to pass into the format string: the logs may be erroneous.");
  }
  return format_arg_count;
}

#endif // LOGGER_IMPL_H
