#include "encrypt.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "addRoundKey.h"
#include "keySchedule.h"
#include "matrix.h"
#include "mixColumns.h"
#include "rCon.h"
#include "rotWord.h"
#include "shiftRows.h"
#include "subBytes.h"

#define BUFFER_LENGTH 16
#define NUMBER_OF_ROUNDS 10

void encrypt(const char* source_file_path, const char* secret_key) {
    if (source_file_path == NULL || secret_key == NULL)
        return;

    unsigned char (*keys[NUMBER_OF_ROUNDS + 1])[4];
    keys[0] = createMatrix(secret_key);

    for (int round = 1; round <= NUMBER_OF_ROUNDS; round++) {
        keys[round] = malloc(SIZE_MATRIX * SIZE_MATRIX * sizeof(unsigned char));
        if (keys[round] == NULL) {
            for (int i = 0; i < round; i++) free(keys[i]);
            return;
        }

        copyMatrix(keys[round], keys[round-1]);
        rotWord(keys[round]);
        subWord(keys[round]);
        rCon(keys[round], round-1);
        keySchedule(keys[round]);
    }

    FILE *file = fopen(source_file_path, "rb");
    char buffer[BUFFER_LENGTH];
    if (file != NULL) {
        size_t bytes_read;
        while ((bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_LENGTH, file)) > 0) {
            unsigned char (*matrix)[4] = createMatrix(buffer);
            if (matrix == NULL)
                break;

            addRoundKey(matrix, keys[0]);

            for (int round = 1; round < NUMBER_OF_ROUNDS; round++) {
                subBytes(matrix);
                shiftRows(matrix);
                mixColumns(matrix);
                addRoundKey(matrix, keys[round]);
            }

            subBytes(matrix);
            shiftRows(matrix);
            addRoundKey(matrix, keys[10]);

            free(matrix);
        }
        fclose(file);
    }

    for (int round = 0; round <= NUMBER_OF_ROUNDS; round++) {
        free(keys[round]);
        keys[round] = NULL;
    }
}
