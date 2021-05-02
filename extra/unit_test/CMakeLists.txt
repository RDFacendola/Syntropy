cmake_minimum_required(VERSION 3.0)

project(unit_test)

# sub-directories

add_subdirectory(../../syntropy syntropy)

# simple_dependency

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-std=c++20")

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(unit_test
   src/unit_test/unit_test.cpp
)

# Dependencies

target_link_libraries(unit_test PUBLIC syntropy)
