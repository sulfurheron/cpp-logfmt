
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
  logFmt.insert("long_long", -((long long)1 << 40));
  logFmt.insert("unsigned_long_long", (unsigned long long)1 << 40);

  // Test overwriting old values
  logFmt.insert("int", 5);

  std::string stringifyResult = logFmt.stringify();

  std::string correctResult("bool_false=false bool_true=true float=12.345000 int=5 "
                            "long_long=-1099511627776 string=\"some text\" unsigned_long_long=1099511627776");

  if (stringifyResult.compare(correctResult) == 0) {
    std::cout << "test correct" << std::endl;

  } else {
    std::cout << "test failed" << std::endl;
    std::cout << "expected: " << correctResult << std::endl;
    std::cout << "     got: " << stringifyResult << std::endl;
    return 1;
  }

  std::cout << "end" << std::endl;

  std::cin.ignore();
  return 0;
}
