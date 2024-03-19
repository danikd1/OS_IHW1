cmake_minimum_required(VERSION 3.26)
project(ihw_v5 C)

set(CMAKE_C_STANDARD 11)

add_executable(ihw_v5 main.c
        textHandler.cpp
        textHandler.c)
