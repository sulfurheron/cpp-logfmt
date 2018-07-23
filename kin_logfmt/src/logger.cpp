#include <boost/date_time/posix_time/posix_time.hpp>

#include "logger.h"

using namespace boost::posix_time;
using namespace kin_logfmt;


Logger::Logger(LEVEL log_level, FileStream *stream, const std::string& module)
  : log_level_(log_level),
    stream_(stream), 
    module_name_(module) {}

void Logger::FATAL(const std::string& msg, ...) {
  if (FATAL_ < log_level_) {
    return;
  }
  va_list args;
  va_start(args, msg);
  write(msg, args);
  va_end(args);
}

void Logger::ERROR(const std::string& msg, ...) {
  if (ERROR_ < log_level_) {
    return;
  }
  va_list args;
  va_start(args, msg);
  write(msg, args);
  va_end(args);
}

void Logger::WARN(const std::string& msg, ...) {
  if (WARN_ < log_level_) {
    return;
  }
  va_list args;
  va_start(args, msg);
  write(msg, args);
  va_end(args);
}

void Logger::WARNING(const std::string& msg, ...) {
  if (WARNING_ < log_level_) {
    return;
  }
  va_list args;
  va_start(args, msg);
  write(msg, args);
  va_end(args);
}

void Logger::INFO(const std::string& msg, ...) {
  if (INFO_ < log_level_) {
    return;
  }
  va_list args;
  va_start(args, msg);
  write(msg, args);
  va_end(args);
}

void Logger::DEBUG(const std::string& msg, ...) {
  if (DEBUG_ < log_level_) {
    return;
  }
  va_list args;
  va_start(args, msg);
  write(msg, args);
  va_end(args);
}

void Logger::write(const std::string& msg, va_list& args) {
  std::string metadata = compile_logfmt_metadata();
  std::string content = compile_logfmt_content(msg, args);

  auto out = metadata + " " + content + "\n";
  stream_->write(out);
}

std::string Logger::compile_logfmt_content(const std::string& msg, va_list& args) {
  LogFmtMessage content_msg(&fn_default_key_order);

  // Count the number of actual arguments
  int arg_count = 0;
  auto arg = (std::string) msg;
  va_list args_to_count;
  va_copy(args, args_to_count);
  while (arg != "") {
    arg = va_arg(args_to_count, std::string);
    arg_count++;
  }
  va_end(args_to_count);

  char msg_buf[MAX_MSG_LENGTH];
  int format_arg_count;
  try {
    format_arg_count = sprintf(msg_buf, msg.c_str(), arg_count, args);
    content_msg.insert(to_string(LOGFMT_KEY::msg), msg_buf);

    if (format_arg_count && (arg_count - format_arg_count) % 2 != 0) {
      throw LogfmtException("Key-value pairs are not even; the raw log will be printed");
    }

    for (int i = 0; i < arg_count-format_arg_count; i+=2) {
      auto key = va_arg(args, std::string);
      // if (std::is_enum<decltype(key)>::value) {
      //   content_msg.insert(key, va_arg(args, std::string));
      // } else {
      content_msg.insert(key, va_arg(args, std::string));
      //    }
    }

  } catch (SprintfException &e) {
    std::cerr << e.what(); // TODO(JF): be more descriptive
  }

  return content_msg.stringify();
}

std::string Logger::compile_logfmt_metadata() {
  LogFmtMessage metadata_msg(&fn_default_key_order);

  ptime t = microsec_clock::universal_time();
  auto timestamp = to_iso_extended_string(t) + "Z";

  metadata_msg.insert(to_string(LOGFMT_KEY::module), module_name_);
  metadata_msg.insert(to_string(LOGFMT_KEY::timestamp), timestamp);

  return metadata_msg.stringify();
}

/*
  Sprintf (overloaded) performs some basic tests on the formatting arguments
  to try and throw a descriptive error. It also returns the number of arguments
  it expectedly formatted (based on the number of format descriptors %), rather than
  the number of characters it formatted as sprintf does.
*/
int Logger::sprintf(char *buffer, const char *format, const int arg_count, va_list& args) {

  // Scan the number of expected arguments
  int format_arg_count = (*format == '%') ? 1 : 0;
  for (const char *ch = format; *ch != '\0'; ) {
    if (*ch++ != '%' && *ch == '%') {
      format_arg_count++;
    }
  }

  //int num_chars_formatted =
  vsprintf(buffer, format, args);

  if (arg_count < format_arg_count) {
    throw SprintfException("Not enough arguments to pass into the format string: the logs may be erroneous.");
  }
  return format_arg_count;
}
