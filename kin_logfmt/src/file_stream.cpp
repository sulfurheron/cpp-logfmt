#include <iostream>

#include "file_stream.h"

using namespace kin_logfmt;

void StdErr::write(const std::string &out) {
  std::cerr << out.c_str();
}

void FakeFileStream::write(const std::string &out) {
  messages_.push_back(out);
}
