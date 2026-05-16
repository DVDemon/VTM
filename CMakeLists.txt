cmake_minimum_required(VERSION 3.16)

project(VTM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" ON)

add_subdirectory(VirtualTuringMachine/)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
