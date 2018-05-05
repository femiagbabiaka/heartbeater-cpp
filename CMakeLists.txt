cmake_minimum_required(VERSION 3.10)
project(heartbeater_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(source)


set(BUILD_CPR_TESTS off)
add_subdirectory(include/cpr)

add_library(heartbeater_cpp STATIC
        source/heartbeater.cc
        source/heartbeater.h)

include_directories(${CPR_INCLUDE_DIRS})
target_link_libraries(heartbeater_cpp ${CPR_LIBRARIES})


add_executable(test_executable source/test.cc)
target_link_libraries(test_executable heartbeater_cpp)

