cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_C_COMPILER gcc  CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER g++  CACHE PATH "" FORCE)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(chs-simulator)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Build library
set(LIB_FILES src/api.cpp src/basiccache.cpp src/configparser.cpp src/core.cpp src/datasetparser.cpp src/system.cpp src/replacementpolicy/firstinfirstout.cpp src/replacementpolicy/lastinfirstout.cpp)
add_library(simulator STATIC ${LIB_FILES})
target_include_directories(simulator PRIVATE include/ include/replacementpolicy/ ../third_party/)

# Build unit tests
set(TEST_FILES test/test.cpp test/test_api.cpp test/test_basiccache.cpp test/test_core.cpp test/test_firstinfirstout.cpp test/test_lastinfirstout.cpp test/test_system.cpp)
add_executable(test_simulator ${TEST_FILES})
target_include_directories(test_simulator PRIVATE ../third_party/ include/ include/replacementpolicy/)
target_link_libraries(test_simulator simulator)

# For installing to bin
set(CMAKE_INSTALL_PREFIX ../) 
install(TARGETS simulator test_simulator DESTINATION bin)