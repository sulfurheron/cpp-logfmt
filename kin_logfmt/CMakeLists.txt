PROJECT(kin_logfmt)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

SET(EXECUTABLE_OUTPUT_PATH ".")

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_definitions(-std=c++14 -O2 -pthread -fopenmp -Wall -Wextra)

include_directories(./include
        ../cpp-logfmt/src)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(logger
    include/logger_impl.h
    src/logger.cpp
    src/file_stream.cpp)
target_link_libraries(logger ${Boost_LIBRARIES})

add_library(kin_logfmt
    src/kin_logfmt.cpp)
target_link_libraries(kin_logfmt logger)

install(TARGETS kin_logfmt logger DESTINATION lib)
install(FILES include/kin_logfmt.h
              include/logger.h
              include/logger_impl.h
              include/file_stream.h
              include/logfmt_kv.h
        DESTINATION include/kin_logfmt)

#############
## Testing ##
#############

add_executable(logger_test
    test/test_logger.cpp)
target_link_libraries(logger_test logger ${GTEST_LIBRARIES} pthread)
install(TARGETS logger_test DESTINATION bin)
