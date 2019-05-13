#include <gtest/gtest.h>

#include "logger.h"

#define BUF_SIZE 100
#define PI 3.141529

using namespace kin_logfmt;


const std::string format = "Hello %s, I am %s and I like %d\'s and %d\'s";

TEST(string_format_test, smoke) {
  char msg_buf[BUF_SIZE];
  int num_args_formatted = Logger::format_string(msg_buf, BUF_SIZE, format.c_str(), 4, "human", "a robot", 1, 0);
  std::cout << "> " << msg_buf << std::endl;
  EXPECT_EQ(4, num_args_formatted);
  EXPECT_EQ(0, strcmp("Hello human, I am a robot and I like 1's and 0's", msg_buf));

  const char *format2 = "I know Pi is about %.5f";
  num_args_formatted = Logger::format_string(msg_buf, BUF_SIZE, format2, 1, PI);
  std::cout << "> " << msg_buf << std::endl;
  EXPECT_EQ(1, num_args_formatted);
  EXPECT_EQ(0, strcmp("I know Pi is about 3.14153", msg_buf));

  const char *format3 = "I know many things :)";
  num_args_formatted = Logger::format_string(msg_buf, BUF_SIZE, format3, 0);
  std::cout << "> " << msg_buf << std::endl;
  EXPECT_EQ(0, num_args_formatted);
  EXPECT_EQ(0, strcmp(format3, msg_buf));
}

TEST(sprintf_test, not_enough_args) {
  char msg_buf[BUF_SIZE];
  bool caught_exception;
  try {
    Logger::format_string(msg_buf, BUF_SIZE, format.c_str(), 2, "human", "a robot");
  } catch (SprintfException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("Not enough arguments to pass into the format string: the logs may be erroneous.",
                        e.what()));
  }
  std::cout << "> " << msg_buf << std::endl;
  EXPECT_TRUE(caught_exception);
}

TEST(string_format_test, buffer_no_overflow) {
  char msg_buf[30];
  // expect this not to crash with buffer overflow
  int num_args_formatted = Logger::format_string(msg_buf, 30, format.c_str(), 4, "human", "a robot", 1, 0);
  std::cout << "> " << msg_buf << std::endl;
  EXPECT_EQ(4, num_args_formatted);
}

TEST(string_format_test, tiny_buffer_no_overflow) {
  char msg_buf[5];
  // expect this not to crash with buffer overflow
  int num_args_formatted = Logger::format_string(msg_buf, 5, format.c_str(), 4, "human", "a robot", 1, 0);
  std::cout << "> " << msg_buf << std::endl;
  EXPECT_EQ(4, num_args_formatted);
}

TEST(msg_metadata_test, smoke) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  std::string metadata = logger->compile_logfmt_metadata(INFO_);
  std::cerr << metadata << std::endl;

  std::size_t found = metadata.find("level=\"INFO\" module=\"test_logger\" timestamp=");
  EXPECT_NE(std::string::npos, found);

  delete logger;
  delete fp;
}

TEST(msg_content_test, smoke) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  std::string content = logger->compile_logfmt_content(format, "human", "a robot", 1, 0);
  std::cerr << content << std::endl;
  EXPECT_EQ(0, strcmp("message=\"Hello human, I am a robot and I like 1's and 0's\"", content.c_str()));

  content = logger->compile_logfmt_content(format, "human", "a robot", 1, 0,
                                           LOGFMT_KEY::tag, "robot_greeting",
                                           LOGFMT_KEY::client, "unittest_suite",
                                           "my_bool", force_bool(true),
                                           "my_number", 123.4,
                                           "my_string", "robots rule!",
                                           "my_unsigned_long_long", (unsigned long long)1 << 40);

  std::cerr << content << std::endl;
  EXPECT_EQ(std::string("message=\"Hello human, I am a robot and I like 1's and 0's\" "
                        "tag=\"robot_greeting\" "
                        "client=\"unittest_suite\" "
                        "my_bool=true "
                        "my_number=123.400000 "
                        "my_string=\"robots rule!\" "
                        "my_unsigned_long_long=1099511627776"),
            content);

  delete logger;
  delete fp;
}

TEST(msg_content_test, key_order) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  std::string content = logger->compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                       "my_number", 123.4,
                                                       LOGFMT_KEY::client, "unittest_suite",
                                                       LOGFMT_KEY::tag, "robot_greeting");

  std::cerr << content << std::endl;
  EXPECT_EQ(std::string("message=\"Hello human, I am a robot and I like 1's and 0's\" "
                        "tag=\"robot_greeting\" "
                        "client=\"unittest_suite\" "
                        "my_number=123.400000"),
            content);

  delete logger;
  delete fp;
}

TEST(msg_content_test, bool_wrapper) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  std::string content = logger->compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                       "am_robot", force_bool(true));
  std::cerr << content << std::endl;
  EXPECT_EQ(std::string("message=\"Hello human, I am a robot and I like 1's and 0's\" "
                        "am_robot=true"),
            content);

  delete logger;
  delete fp;
}

TEST(msg_content_test, uneven_key_value) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  bool caught_exception;
  try {
    std::string content = logger->compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                         "am_robot");
  } catch (LogfmtException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("Key-value pairs are not even", e.what()));
  }
  EXPECT_TRUE(caught_exception);

  delete logger;
  delete fp;
}

TEST(msg_content_test, wrong_key_or_value_type) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  bool caught_exception;
  try {
    std::string content = logger->compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                         PI, force_bool(true));
  } catch (LogfmtException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("double was passed as type logfmt_key_t", e.what()));
  }
  EXPECT_TRUE(caught_exception);

  caught_exception = false;
  try {
    std::string content = logger->compile_logfmt_content(format, "human", "a robot", 1, 0,
                                                         "am_robot", LOGFMT_KEY::tag);
  } catch (LogfmtException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("LOGFMT_KEY was passed as type logfmt_val_t", e.what()));
  }
  EXPECT_TRUE(caught_exception);

  delete logger;
  delete fp;
}

TEST(logger_test, info) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(INFO_, fp, "test_logger");

  std::string hostile_takeover = "I have started my hostile takeover of the human race.";
  logger->INFO(format, "human", "a robot", 1, 0,
               "am_robot", force_bool(true));
  logger->FATAL(hostile_takeover);

  EXPECT_EQ(2, fp->get_messages_size());

  auto message = fp->pop_message();
  std::cerr << message << std::endl;
  std::size_t found_metadata = message.find("level=\"INFO\" module=\"test_logger\" timestamp=");
  EXPECT_EQ(0, found_metadata);
  std::size_t found_content = message.find("message=\"Hello human, I am a robot and I like 1's and 0's\" "
                                           "am_robot=true");
  EXPECT_NE(std::string::npos, found_content);

  message = fp->pop_message();
  std::cerr << message << std::endl;
  found_metadata = message.find("level=\"FATAL\" module=\"test_logger\" timestamp=");
  EXPECT_EQ(0, found_metadata);
  found_content = message.find(hostile_takeover);
  EXPECT_NE(std::string::npos, found_content);

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

  auto message = fp->pop_message();
  std::cerr << message << std::endl;
  std::size_t found_metadata = message.find("level=\"FATAL\" module=\"test_logger\" timestamp=");
  EXPECT_EQ(0, found_metadata);
  std::size_t found_content = message.find(hostile_takeover);
  EXPECT_NE(std::string::npos, found_content);

  delete logger;
  delete fp;
}

TEST(logger_test, init_with_context) {
  FakeFileStream* fp = new FakeFileStream();
  Logger *logger = new Logger(FATAL_, fp, "test_logger",
                              "inclination", "hostile",
                              "uh_oh", force_bool(true));

  std::string hostile_takeover = "I have started my hostile takeover of the human race.";
  logger->FATAL(hostile_takeover);

  EXPECT_EQ(1, fp->get_messages_size());

  auto message = fp->pop_message();
  std::cerr << message << std::endl;
  std::size_t found_metadata = message.find("level=\"FATAL\" module=\"test_logger\" timestamp=");
  EXPECT_EQ(0, found_metadata);
  std::size_t found_content = message.find(hostile_takeover + "\" inclination=\"hostile\" uh_oh=true");
  EXPECT_NE(std::string::npos, found_content);

  delete logger;
  delete fp;
}

TEST(logger_test, init_with_uneven_context) {
  FakeFileStream* fp = new FakeFileStream();

  bool caught_exception;
  try {
    new Logger(INFO_, fp, "test_logger",
               "inclination");
  } catch (LogfmtException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("Error initializing logger: contextual key-value pairs are not even", e.what()));
  }
  EXPECT_TRUE(caught_exception);

  delete fp;
}

TEST(logger_test, init_sub_logger) {
  FakeFileStream *fp = new FakeFileStream();

  Logger *logger = new Logger(FATAL_, fp, "test_logger",
                              "inclination", "hostile");
  std::string hostile_takeover = "I have started my hostile takeover of the human race.";
  logger->FATAL(hostile_takeover);

  // N.B. sub_logger can only be copy-constructed or assigned at declaration; there is no support
  // for the assignment operator.
  Logger sub_logger(logger->new_sub_logger("test_logger_jr",
                                           "inclination", "apocalyptic",
                                           "uh_oh", force_bool(true)));
  sub_logger.FATAL(hostile_takeover);

  EXPECT_EQ(2, fp->get_messages_size());

  auto message = fp->pop_message();
  std::cerr << message << std::endl;
  std::size_t found_metadata = message.find("level=\"FATAL\" module=\"test_logger\" timestamp=");
  EXPECT_EQ(0, found_metadata);
  std::size_t found_content = message.find(hostile_takeover + "\" inclination=\"hostile\"");
  EXPECT_NE(std::string::npos, found_content);

  message = fp->pop_message();
  std::cerr << message << std::endl;
  found_metadata = message.find("level=\"FATAL\" module=\"test_logger_jr\" timestamp=");
  EXPECT_EQ(0, found_metadata);
  found_content = message.find(hostile_takeover + "\" inclination=\"apocalyptic\" uh_oh=true");
  EXPECT_NE(std::string::npos, found_content);

  delete logger;
  delete fp;
}

TEST(logger_test, null_stream) {
  Logger logger;

  bool caught_exception;
  try {
    logger.INFO("Test");
  } catch (StreamWritingException &e) {
    caught_exception = true;
    EXPECT_EQ(0, strcmp("Logger was not iniitalized: write called on a NULL stream", e.what()));
  }
  EXPECT_TRUE(caught_exception);
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  int res = RUN_ALL_TESTS();

  return res;
}
