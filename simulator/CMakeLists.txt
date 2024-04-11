# TODO: Finish
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(chs-simulator)

set(SOURCE_FILES src/api.cpp src/basiccache.cpp src/core.cpp src/system.cpp src/replacementpolicy/firstinfirstout.cpp src/replacementpolicy/lastinfirstout.cpp)
add_library(simulator STATIC ${SOURCE_FILES})
target_include_directories(simulator PRIVATE include/ include/replacementpolicy/)


set(TEST_FILES test/test.cpp test/test_basiccache.cpp)
add_executable(test_simulator ${TEST_FILES})
target_include_directories(test_simulator PRIVATE ../third_party/ include/ include/replacementpolicy/)
target_link_libraries(test_simulator simulator)