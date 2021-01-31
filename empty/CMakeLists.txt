cmake_minimum_required(VERSION 3.0)

project(empty)

set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(empty
   src/empty/main.cpp
)
