#include "../include/encrypt.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../include/addRoundKey.h"
#include "../include/keySchedule.h"
#include "../include/matrix.h"
#include "../include/mixColumns.h"
#include "../include/rCon.h"
#include "../include/rotWord.h"
#include "../include/shiftRows.h"
#include "../include/subBytes.h"

#define BUFFER_LENGTH 16
#define NUMBER_OF_ROUNDS 10

char* encrypt(const char* text, const char* secret_key) {
    if (text == NULL || secret_key == NULL)
        return NULL;

    const size_t text_length = strlen(text);
    size_t num_blocks = (text_length / BUFFER_LENGTH) + (text_length % BUFFER_LENGTH != 0 ? 1 : 0);
    if (num_blocks == 0) num_blocks = 1;

    const size_t total_encrypted_length = num_blocks * BUFFER_LENGTH;
    char* encrypted_text = malloc((total_encrypted_length + 1) * sizeof(char));
    if (encrypted_text == NULL)
        return NULL;

    unsigned char (*keys[NUMBER_OF_ROUNDS + 1])[4];
    keys[0] = createMatrix(secret_key, BUFFER_LENGTH);

    for (int round = 1; round <= NUMBER_OF_ROUNDS; round++) {
        keys[round] = malloc(SIZE_MATRIX * SIZE_MATRIX * sizeof(unsigned char));
        if (keys[round] == NULL) {
            for (int i = 0; i < round; i++) free(keys[i]);
            free(encrypted_text);
            return NULL;
        }

        copyMatrix(keys[round], keys[round-1]);
        rotWord(keys[round]);
        subWord(keys[round]);
        rCon(keys[round], round-1);
        keySchedule(keys[round], keys[round-1]);
    }

    char buffer[BUFFER_LENGTH + 1];
    int current_offset = 0;

    for (int length_string = 0; length_string < strlen(text); length_string+=BUFFER_LENGTH) {
        const size_t remaining_bytes = text_length - length_string;
        const size_t bytes_to_copy = remaining_bytes < BUFFER_LENGTH ? remaining_bytes : BUFFER_LENGTH;

        memset(buffer, 0, BUFFER_LENGTH + 1);
        strncpy(buffer, text + length_string, bytes_to_copy);

        unsigned char (*matrix)[4] = createMatrix(buffer, BUFFER_LENGTH);
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

        char* chunk_string = printMatrix(matrix);
        if (chunk_string != NULL) {
            memcpy(encrypted_text + current_offset, chunk_string, BUFFER_LENGTH);
            current_offset += BUFFER_LENGTH;

            free(chunk_string);
        }

        free(matrix);
    }
    encrypted_text[total_encrypted_length] = '\0';

    for (int round = 0; round <= NUMBER_OF_ROUNDS; round++) {
        free(keys[round]);
        keys[round] = NULL;
    }

    return encrypted_text;
}
