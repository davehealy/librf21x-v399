cmake_minimum_required(VERSION 2.8)

add_library(rf21x STATIC
    rf21x.cpp rf21x-base.cpp rf219.cpp rf218.cpp rf217.cpp rf215.cpp rf228.cpp)
target_link_libraries(rf21x gsp)
include_directories(../tthread)
include_directories(../gsp)


option(BUILD_BASIC_TEST "Build test for basic rf21x c++ implementation. This is only for debuging of this library." OFF)
if(NOT BUILD_BASIC_TEST)
    return()
endif()

add_executable(rf21x-test rf21x-test.cpp)
target_link_libraries(rf21x-test rf21x)

install(TARGETS rf21x-test DESTINATION .)
