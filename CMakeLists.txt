cmake_minimum_required(VERSION 3.10)

project(VTM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

add_subdirectory(VirtualTuringMachine/)
