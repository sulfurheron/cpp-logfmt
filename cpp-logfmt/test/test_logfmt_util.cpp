
#include <iostream>
#include <string>

#include "Logfmt.h"

bool fn_alpha_key_order(const std::string lhs, const std::string rhs) {
  return lhs < rhs;
}

int main() {

  std::cout << "starting test" << std::endl;

  LogFmtMessage logFmt(&fn_alpha_key_order);

  logFmt.insert("string", "some text");
  logFmt.insert("bool_true", true);
  logFmt.insert("bool_false", false);
  logFmt.insert("int", 10);
  logFmt.insert("float", 12.345f);

  std::string stringifyResult = logFmt.stringify();

  std::string correctResult("bool_false=false bool_true=true float=12.345000 int=10 string=\"some text\"");

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
