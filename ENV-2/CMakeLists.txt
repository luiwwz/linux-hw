cmake_minimum_required(VERSION 3.10)
project(ComplexSorting)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES main.cpp Complex.cpp Sort.cpp)

add_executable(program ${SOURCES})
