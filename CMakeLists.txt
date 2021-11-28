cmake_minimum_required(VERSION 3.20)
project(sysop)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Werror -Wall -pedantic)
add_executable(sysop main.cpp)
