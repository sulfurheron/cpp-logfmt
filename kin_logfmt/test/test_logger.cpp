#include <gtest/gtest.h>

#include "logger.h"

#define MAX_MSG_LENGTH 100

using namespace kin_logfmt;


int sprintf_arg_wrapper(char *buffer, const char* format, const int argc, ...) {
  va_list args;
  va_start(args, argc);
  int num_args_formatted = Logger::sprintf(buffer, format, argc, args);
  va_end(args);

  return num_args_formatted;
}


TEST(sprintf_test, smoke) {
  char msg_buf[MAX_MSG_LENGTH];
  const char *format = "Hello %s, I am %s and I like %d\'s and %d\'s\n";

  int num_args_formatted = sprintf_arg_wrapper(msg_buf, format, 4, "human", "a robot", 1, 0);
  std::cerr << msg_buf;
  EXPECT_EQ(4, num_args_formatted);
  EXPECT_EQ("Hello human, I am a robot and I like 1's and 0's", format);
}

TEST(sprintf_test, not_enough_args) {

}

TEST(sprintf_test, no_args) {

}

TEST(msg_metadata_test, smoke) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  delete logger;
  delete fp;
}

TEST(msg_content_test, smoke) {

}

TEST(msg_content_test, uneven_key_value) {

}

TEST(logger_test, fatal) {

}

TEST(logger_test, info) {

}

TEST(logger_test, level_filter) {

}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  int res = RUN_ALL_TESTS();

  return res;
}
