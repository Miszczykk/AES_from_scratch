cmake_minimum_required(VERSION 4.0)
project(AES_from_scratch C)

set(CMAKE_C_STANDARD 11)

add_executable(AES_from_scratch main.c
        matrix.c
        matrix.h)
