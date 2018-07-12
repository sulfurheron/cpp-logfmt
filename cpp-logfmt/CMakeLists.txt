PROJECT(cpp-logfmt)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

SET(EXECUTABLE_OUTPUT_PATH ".")

add_definitions(-std=c++14 -g -O2 -pthread -fopenmp -Wall -Wextra)

include_directories(./src)

add_executable(test_logfmt
        src/Logfmt.h
        src/test.cpp)
install(TARGETS test_logfmt DESTINATION bin)
