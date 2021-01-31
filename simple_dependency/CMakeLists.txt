cmake_minimum_required(VERSION 3.0)

project(simple_dependency)

include_directories(${PROJECT_SOURCE_DIR}/../simple_lib/include/)

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_subdirectory(../simple_lib simple_lib)

set(CMAKE_CXX_COMPILER "/Users/daniele/Desktop/clangm1/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++20")

add_executable(simple_dependency
   src/simple_dependency/simple_dependency.cpp
)

target_link_libraries(simple_dependency PUBLIC simple_lib)
