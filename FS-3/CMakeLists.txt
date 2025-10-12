cmake_minimum_required(VERSION 3.5)
project(hacker-rm LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 17)
set(SOURCES hacker-rm.cpp)

add_executable(hacker-rm ${SOURCES})
