cmake_minimum_required(VERSION 3.10)

# set the project name
project(PSOracle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add the executable
add_library(PSOracle src/convert.cc)
target_link_libraries(PSOracle PUBLIC mockturtle OpenDB OpenSTA)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
add_executable(runUnitTests tests/basic.cpp)
target_link_libraries(runUnitTests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME runUnitTests
  COMMAND runUnitTests)
