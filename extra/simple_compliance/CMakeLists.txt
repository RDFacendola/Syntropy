cmake_minimum_required(VERSION 3.0)

project(simple_compliance)

# sub-directories

add_subdirectory(../../syntropy syntropy)

# simple_dependency

set(CMAKE_CXX_COMPILER "/Users/daniele/Desktop/clangm1/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++20")

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(simple_compliance
   src/simple_compliance/simple_compliance.cpp
)

# Dependencies

target_link_libraries(simple_compliance PUBLIC syntropy)
