
#include <iostream>
#include <unordered_map>
#include <string>

#include "Logfmt.h"

int main() {

  std::cout << "starting test" << std::endl;

  LogFmtMessage logFmt;

  logFmt.insert("string", "some text");
  logFmt.insert("bool_true", true);
  logFmt.insert("bool_false", false);
  logFmt.insert("int", 10);
  logFmt.insert("float", 12.345f);

  std::string stringifyResult = logFmt.stringify();

  // this is a crappy test since unordered map obviously doesnt have any order
  std::string correctResult("string=\"some text\" bool_true=true float=12.345000 bool_false=false int=10");

  if (stringifyResult.compare(correctResult) == 0) {
    std::cout << "test correct" << std::endl;
  } else {
    std::cout << "test failed" << std::endl;
    std::cout << "expected: " << correctResult << std::endl;
    std::cout << "     got: " << stringifyResult << std::endl;
  }

  std::cout << "end" << std::endl;

  std::cin.ignore();

}