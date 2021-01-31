cmake_minimum_required(VERSION 3.0)

project(simple_cxx20)

set(CMAKE_CXX_COMPILER "/Users/daniele/Desktop/clangm1/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++20")

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(simple_cxx20
   src/simple_cxx20/main.cpp
)
