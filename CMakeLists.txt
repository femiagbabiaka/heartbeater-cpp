cmake_minimum_required(VERSION 3.10)
project(heartbeater_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(source)


set(BUILD_CPR_TESTS off)
add_subdirectory(include/cpr)

add_executable(heartbeater_cpp
        source/heartbeater.cc
        source/heartbeater.h
        source/test.cc)

include_directories(${CPR_INCLUDE_DIRS})
target_link_libraries(heartbeater_cpp ${CPR_LIBRARIES})



