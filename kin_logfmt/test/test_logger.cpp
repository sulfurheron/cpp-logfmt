#include <gtest/gtest.h>

#include "logger.h"

#define MAX_MSG_LENGTH 100
#define PI 3.141529

using namespace kin_logfmt;


const std::string format = "Hello %s, I am %s and I like %d\'s and %d\'s";

TEST(sprintf_test, smoke) {
  char msg_buf[MAX_MSG_LENGTH];
  int num_args_formatted = Logger::sprintf(msg_buf, format.c_str(), 4, "human", "a robot", 1, 0);
  std::cerr << msg_buf << std::endl;
  EXPECT_EQ(4, num_args_formatted);
  EXPECT_EQ(0, strcmp("Hello human, I am a robot and I like 1's and 0's", msg_buf));

  const char *format2 = "I know Pi is about %.5f";
  num_args_formatted = Logger::sprintf(msg_buf, format2, 1, PI);
  std::cerr << msg_buf << std::endl;
  EXPECT_EQ(1, num_args_formatted);
  EXPECT_EQ(0, strcmp("I know Pi is about 3.14153", msg_buf));

  const char *format3 = "I know many things :)";
  num_args_formatted = Logger::sprintf(msg_buf, format3, 0);
  std::cerr << msg_buf << std::endl;
  EXPECT_EQ(0, num_args_formatted);
  EXPECT_EQ(0, strcmp(format3, msg_buf));
}

TEST(sprintf_test, not_enough_args) {
  char msg_buf[MAX_MSG_LENGTH];
  bool caught_exception;
  try {
    Logger::sprintf(msg_buf, format.c_str(), 2, "human", "a robot");
  } catch (SprintfException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("Not enough arguments to pass into the format string: the logs may be erroneous.",
                        e.what()));
  }
  std::cerr << msg_buf << std::endl;
  EXPECT_TRUE(caught_exception);
}

TEST(msg_metadata_test, smoke) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  std::string metadata = logger->compile_logfmt_metadata();
  std::cerr << metadata << std::endl;

  std::size_t found = metadata.find("module=\"test_logger\" timestamp=");
  EXPECT_NE(std::string::npos, found);

  delete logger;
  delete fp;
}

TEST(msg_content_test, smoke) {
  std::string content = Logger::compile_logfmt_content(format, "human", "a robot", 1, 0);
  std::cerr << content << std::endl;
  EXPECT_EQ(0, strcmp("msg=\"Hello human, I am a robot and I like 1's and 0's\"", content.c_str()));

  content = Logger::compile_logfmt_content(format, "human", "a robot", 1, 0,
                                           LOGFMT_KEY::tag, "robot_greeting",
                                           LOGFMT_KEY::client, "unittest_suite",
                                           "my_number", 123.4);
  std::cerr << content << std::endl;
  EXPECT_EQ(std::string("msg=\"Hello human, I am a robot and I like 1's and 0's\" "
                        "tag=\"robot_greeting\" "
                        "client=\"unittest_suite\" "
                        "my_number=123.400000"),
            content);
}

TEST(msg_content_test, key_order) {
  std::string content = Logger::compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                       "my_number", 1234,
                                                       LOGFMT_KEY::client, "unittest_suite",
                                                       LOGFMT_KEY::tag, "robot_greeting");

  std::cerr << content << std::endl;
  EXPECT_EQ(std::string("msg=\"Hello human, I am a robot and I like 1's and 0's\" "
                        "tag=\"robot_greeting\" "
                        "client=\"unittest_suite\" "
                        "my_number=1234"),
            content);
}

TEST(msg_content_test, bool_wrapper) {
  std::string content = Logger::compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                       "am_robot", force_bool(true));
  std::cerr << content << std::endl;
  EXPECT_EQ(std::string("msg=\"Hello human, I am a robot and I like 1's and 0's\" "
                        "am_robot=true"),
            content);
}

TEST(msg_content_test, uneven_key_value) {
  bool caught_exception;
  try {
    std::string content = Logger::compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                         "am_robot");
  } catch (LogfmtException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("Key-value pairs are not even", e.what()));
  }
  EXPECT_TRUE(caught_exception);
}

TEST(msg_content_test, wrong_key_or_value_type) {
  bool caught_exception;
  try {
    std::string content = Logger::compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                         PI, force_bool(true));
  } catch (LogfmtException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("double was passed as type logfmt_key_t", e.what()));
  }
  EXPECT_TRUE(caught_exception);

  caught_exception = false;
  try {
    std::string content = Logger::compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                         "am_robot", LOGFMT_KEY::tag);
  } catch (LogfmtException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("LOGFMT_KEY was passed as type logfmt_val_t", e.what()));
  }
  EXPECT_TRUE(caught_exception);
}

TEST(logger_test, info) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  std::string hostile_takeover = "I have started my hostile takeover of the human race.";
  logger->INFO(format, "human", "a robot", 1, 0,
               "am_robot", force_bool(true));
  logger->FATAL(hostile_takeover);

  EXPECT_EQ(2, fp->get_messages_size());
  std::size_t found = fp->pop_message().find("Hello human, I am a robot and I like 1's and 0's");
  EXPECT_NE(std::string::npos, found);
  found = fp->pop_message().find(hostile_takeover);
  EXPECT_NE(std::string::npos, found);

  delete logger;
  delete fp;
}

TEST(logger_test, fatal) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(FATAL_, fp, "test_logger");

  std::string hostile_takeover = "I have started my hostile takeover of the human race.";
  logger->INFO(format, "human", "a robot", 1, 0);
  logger->FATAL(hostile_takeover);

  EXPECT_EQ(1, fp->get_messages_size());
  std::size_t found = fp->pop_message().find(hostile_takeover);
  EXPECT_NE(std::string::npos, found);

  delete logger;
  delete fp;
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  int res = RUN_ALL_TESTS();

  return res;
}
