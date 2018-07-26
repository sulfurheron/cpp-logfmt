#include <iostream>

#include "file_stream.h"

using namespace kin_logfmt;

void StdErr::write(const std::string &out) {
  boost::mutex::scoped_lock lock(writers_mtx_);

  std::cerr << out.c_str() << std::endl;
}

void FakeFileStream::write(const std::string &out) {
  boost::mutex::scoped_lock lock(writers_mtx_);

  messages_.push_back(out);
}

int FakeFileStream::get_messages_size() {
  boost::mutex::scoped_lock lock(writers_mtx_);

  return messages_.size();
}

std::string FakeFileStream::pop_message() {
  boost::mutex::scoped_lock lock(writers_mtx_);

  auto message = messages_.front();
  messages_.pop_front();
  return message;
}
