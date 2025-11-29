cmake_minimum_required(VERSION 3.10)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 11)

add_library(parallel_scheduler SHARED parallel_scheduler.cpp)
target_link_libraries(parallel_scheduler pthread)

add_executable(demo demo.cpp)
target_link_libraries(demo parallel_scheduler pthread)
