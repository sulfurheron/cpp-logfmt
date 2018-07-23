PROJECT(kin_logfmt)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

SET(EXECUTABLE_OUTPUT_PATH ".")

find_package(GTest REQUIRED)

add_definitions(-std=c++14 -g -O2 -pthread -fopenmp -Wall -Wextra)

include_directories(./include
        ../cpp-logfmt/src)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(logger
    src/logger.cpp
    src/file_stream.cpp)

add_library(kin_logfmt
    src/kin_logfmt.cpp)
target_link_libraries(kin_logfmt logger)

install(TARGETS kin_logfmt DESTINATION lib)

#############
## Testing ##
#############

add_executable(logger_test
    test/test_logger.cpp)
target_link_libraries(logger_test logger ${GTEST_LIBRARIES} pthread)
install(TARGETS logger_test DESTINATION bin)