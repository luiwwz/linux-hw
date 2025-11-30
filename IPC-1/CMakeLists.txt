cmake_minimum_required(VERSION 3.10)

project(signal_echo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(signal-echo signal-echo.cpp)
