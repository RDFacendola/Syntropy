cmake_minimum_required(VERSION 3.0)

project(simple_lib)

# simple_lib

set(CMAKE_CXX_COMPILER "/Users/daniele/Desktop/clangm1/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++20")

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_library(simple_lib STATIC
   src/simple_lib/simple_lib.cpp
)

# Export

target_include_directories(simple_lib PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
