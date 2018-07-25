#include <boost/date_time/posix_time/posix_time.hpp>

#include "logger.h"

using namespace boost::posix_time;
using namespace kin_logfmt;


Logger::Logger(LEVEL log_level, FileStream *stream, const std::string& module)
  : log_level_(log_level),
    stream_(stream), 
    module_name_(module) {}

std::string Logger::compile_logfmt_metadata(LEVEL level) {
  LogFmtMessage metadata_msg(&fn_default_key_order);

  ptime t = microsec_clock::universal_time();
  auto timestamp = to_iso_extended_string(t) + "Z";

  metadata_msg.insert(to_string(LOGFMT_KEY::level), to_string(level));
  metadata_msg.insert(to_string(LOGFMT_KEY::module), module_name_);
  metadata_msg.insert(to_string(LOGFMT_KEY::timestamp), timestamp);

  return metadata_msg.stringify();
}
