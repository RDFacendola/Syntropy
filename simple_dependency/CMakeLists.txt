cmake_minimum_required(VERSION 3.0)

project(simple_dependency)

# sub-directories

add_subdirectory(../simple_lib simple_lib)

# simple_dependency

set(CMAKE_CXX_COMPILER "/Users/daniele/Desktop/clangm1/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++20")

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(simple_dependency
   src/simple_dependency/simple_dependency.cpp
)

# Dependencies

target_link_libraries(simple_dependency PUBLIC simple_lib)
