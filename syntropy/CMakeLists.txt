cmake_minimum_required(VERSION 3.0)

project(syntropy)

# syntropy

set(CMAKE_CXX_COMPILER "/Users/daniele/Desktop/clangm1/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++20")

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/)

add_library(syntropy STATIC
   src/syntropy/syntropy.cpp
)

# Export

target_include_directories(syntropy PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
