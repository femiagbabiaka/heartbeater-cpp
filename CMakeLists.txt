cmake_minimum_required(VERSION 3.10)
project(heartbeater_cpp)

set(CMAKE_CXX_STANDARD 11)
set(BUILD_CPR_TESTS OFF)

include_directories(source)

macro(heartbeater_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
    if (DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif ()
    if (${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif ()
    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()


heartbeater_option(BUILD_HEARTBEATER_TESTS "Set to ON to build heartbeater tests." ON)

add_subdirectory(include/cpr)

add_library(heartbeater_cpp STATIC
        source/heartbeater.cc
        source/heartbeater.h)

include_directories(${CPR_INCLUDE_DIRS})
target_link_libraries(heartbeater_cpp ${CPR_LIBRARIES})

if (BUILD_HEARTBEATER_TESTS)
    # Add the google test submodule.
    add_subdirectory(include/googletest)
    # Enable testing.
    enable_testing()
    # Include gtest include directory.
    include_directories(${gtest_SOURCE_DIR/include} ${gtest_SOURCE_DIR})
    add_executable(heartbeater_tests source/heartbeater_test.cpp)
    target_link_libraries(heartbeater_tests heartbeater_cpp gtest gtest_main)
    add_test(NAME heartbeater_tests COMMAND heartbeater_tests)
endif ()


