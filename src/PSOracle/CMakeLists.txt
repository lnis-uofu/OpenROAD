cmake_minimum_required(VERSION 3.10)

# set the project name
project(PSOracle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# add the executable
find_package(TCL)
add_library(PSOracle src/convert.cc)
target_include_directories(PSOracle PUBLIC ../../include)
target_link_libraries(PSOracle PUBLIC mockturtle opendb OpenSTA dbSta ${TCL_LIBRARY} ZLIB::ZLIB)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
add_executable(runUnitTests tests/basic.cpp)
target_link_libraries(runUnitTests PSOracle ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
add_test(NAME runUnitTests COMMAND runUnitTests)
