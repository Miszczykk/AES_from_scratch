cmake_minimum_required(VERSION 4.0)
project(AES_from_scratch C)

set(CMAKE_C_STANDARD 11)

add_executable(AES_from_scratch main.c
        matrix.c
        matrix.h
        subBytes.c
        subBytes.h
        shiftRows.c
        shiftRows.h
        rotWord.c
        rotWord.h)
