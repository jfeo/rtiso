cmake_minimum_required(VERSION 2.6)

project(rtiso C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGLEW_STATIC -std=c11 -pedantic")

# Version number
set(rtiso_VERSION_MAJOR 0)
set(rtiso_VERSION_MINOR 0)

# Paths
set(BIN_DIR ${rtiso_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Add the main project
add_subdirectory(${rtiso_SOURCE_DIR}/src)

# TODO: Add test project here
