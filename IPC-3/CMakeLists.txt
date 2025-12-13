cmake_minimum_required(VERSION 3.10)

project(shared_array)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SHARED_SRC shared_array.cpp)

add_executable(first first.cpp ${SHARED_SRC})
add_executable(second second.cpp ${SHARED_SRC})
add_executable(cleanup clean.cpp ${SHARED_SRC})

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "first;second")

