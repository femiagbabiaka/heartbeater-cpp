cmake_minimum_required(VERSION 3.10)
project(heartbeater_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(source)

add_executable(heartbeater_cpp
        source/heartbeater.cc
        source/heartbeater.h
        source/test.cc)
