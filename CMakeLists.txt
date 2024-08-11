cmake_minimum_required(VERSION 3.22)

project(cjson)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")

aux_source_directory(. DIR_SRCS)

add_library(cjson ${DIR_SRCS})
