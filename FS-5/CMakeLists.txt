cmake_minimum_required(VERSION 3.10)
project(ManualAppend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES manual-append.cpp)

add_executable(manual-append ${SOURCES})

