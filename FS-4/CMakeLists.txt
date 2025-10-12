cmake_minimum_required(VERSION 3.5)
project (Redirect)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES redirect.cpp)

add_executable (redirect ${SOURCES})
