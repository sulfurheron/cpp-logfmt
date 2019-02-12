PROJECT(cpp-logfmt)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

SET(EXECUTABLE_OUTPUT_PATH ".")

add_definitions(-std=c++14 -O2 -pthread -fopenmp -Wall -Wextra)

include_directories(./src)

add_executable(test_logfmt_util
        test/test_logfmt_util.cpp
        src/Logfmt.h)
install(TARGETS test_logfmt_util DESTINATION bin)
install(FILES src/Logfmt.h DESTINATION include/kin_logfmt)
