cmake_minimum_required(VERSION 3.10)
project(TCPChat)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE Threads::Threads)

add_executable(client client.cpp)
target_link_libraries(client PRIVATE Threads::Threads)
