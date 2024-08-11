cmake_minimum_required(VERSION 3.22)

project(cjson)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")

add_library(cjson cjson.c cjson.h)

add_executable(test test.c)

target_link_libraries(test  cjson)
