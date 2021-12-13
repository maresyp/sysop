cmake_minimum_required(VERSION 3.20)
project(sysop)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-Werror -Wall -pedantic)
add_executable(sysop main.cpp lab_1/forking_process.cpp lab_1/cloning_process.cpp lab_1/mutex_sync.c lab_2/fork_clone.c lab_2/singleton.c)
