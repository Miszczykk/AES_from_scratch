cmake_minimum_required(VERSION 4.0)
project(AES_from_scratch C)

set(CMAKE_C_STANDARD 11)

add_executable(AES_from_scratch main.c
        src/matrix.c
        include/matrix.h
        src/subBytes.c
        include/subBytes.h
        src/shiftRows.c
        include/shiftRows.h
        src/rotWord.c
        include/rotWord.h
        include/rCon.h
        src/rCon.c
        src/mixColumns.c
        include/mixColumns.h
        src/keySchedule.c
        include/keySchedule.h
        src/addRoundKey.c
        include/addRoundKey.h
        src/encrypt.c
        include/encrypt.h
        src/decryption.c
        include/decryption.h)
