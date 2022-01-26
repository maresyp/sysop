cmake_minimum_required(VERSION 3.20)
project(sysop)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Werror -Wall -pedantic)

#add_executable(sysop lab_5/threading_sha.c lab_5/sha256.c lab_5/sha256.h)
add_executable(sysop lab_4/threading_win.c)
