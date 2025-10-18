cmake_minimum_required(VERSION 3.10)
project(InteractiveShell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES interactive-shell.cpp)

add_executable(interactive-shell ${SOURCES})

