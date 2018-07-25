#ifndef LOGFMT_H
#define LOGFMT_H

#include <sstream>
#include <string>
#include <map>


class LogFmtMessage {
  public:

  LogFmtMessage(bool(*fn_order)(std::string,std::string)) {
    logfmt_kv = std::map<std::string, std::string, bool(*)(std::string,std::string)>(fn_order);
  }

  std::string stringify() {
    std::ostringstream buffer;
    std::map<std::string, std::string>::const_iterator it = logfmt_kv.begin();
    while(it != logfmt_kv.end()) {
      buffer << it->first << EQUALS << it->second;
      ++it;
      if (it != logfmt_kv.end()) buffer << SEPARATOR;
    }
    return buffer.str();
  };

  void insert(const std::string& first, const std::string& second) {
    std::string secondBuffer(second);
    secondBuffer.insert(0, 1, '"');
    secondBuffer.append(1, '"');
    logfmt_kv.insert({ first, secondBuffer });
  };

  void insert(const std::string& first, const char* second) {
    insert(first, std::string(second));
  };

  void insert(const std::string& first, const int& second) {
    logfmt_kv.insert({ first, std::to_string(second) });
  };

  void insert(const std::string& first, const double& second) {
    logfmt_kv.insert({ first, std::to_string(second) });
  };

  void insert(const std::string& first, const bool& second) {
    if (second) {
      logfmt_kv.insert({ first, "true" });
    } else {
      logfmt_kv.insert({ first, "false" });
    }
  };

  private:

  static const char SEPARATOR = ' ';
  static const char EQUALS = '=';

  std::map<std::string, std::string, bool(*)(std::string,std::string)> logfmt_kv;
};

#endif
