#ifndef LOGFMT_H
#define LOGFMT_H

#include <sstream>
#include <string>
#include <map>

class LogFmtMessage {
public:
  std::string stringify() {
    std::ostringstream buffer;
    std::map<std::string, std::string>::const_iterator it = map.begin(); 
    while(it != map.end()) {
      buffer << it->first << EQUALS << it->second;
      ++it;
      if (it != map.end()) buffer << SEPARATOR;
    }
    return buffer.str();
  };

  void insert(const std::string& first, const std::string& second) {
    std::string secondBuffer(second);
    secondBuffer.insert(0, 1, '"');
    secondBuffer.append(1, '"');
    map.insert({ first, secondBuffer });
  };

  void insert(const std::string& first, const char* second) {
    insert(first, std::string(second));
  };

  void insert(const std::string& first, const int& second) {
    map.insert({ first, std::to_string(second) });
  };

  void insert(const std::string& first, const float& second) {
    map.insert({ first, std::to_string(second) });
  };

  void insert(const std::string& first, const bool& second) {
    if (second) {
      map.insert({ first, "true" });
    } else {
      map.insert({ first, "false" });
    }
  };
private:
  static const char SEPARATOR = ' ';
  static const char EQUALS = '=';

  std::map<std::string, std::string> map;
};

#endif
