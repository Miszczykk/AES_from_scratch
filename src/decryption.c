#include "../include/decryption.h"

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

char* decryption(const char* secret_text, const int secret_length, const char* secret_key) {
    if (secret_text == NULL || secret_key == NULL || secret_length == 0)
        return NULL;

    size_t num_blocks = (secret_length / BUFFER_LENGTH) + (secret_length % BUFFER_LENGTH != 0 ? 1 : 0);
    if (num_blocks == 0) num_blocks = 1;

    const size_t total_decrypted_length = num_blocks * BUFFER_LENGTH;
    char* decrypted_text = malloc((total_decrypted_length + 1) * sizeof(char));
    if (decrypted_text == NULL)
        return NULL;

    unsigned char (*keys[NUMBER_OF_ROUNDS + 1])[4];
    keys[0] = createMatrix(secret_key, BUFFER_LENGTH);

    for (int round = 1; round <= NUMBER_OF_ROUNDS; round++) {
        keys[round] = malloc(SIZE_MATRIX * SIZE_MATRIX * sizeof(unsigned char));
        if (keys[round] == NULL) {
            for (int i = 0; i < round; i++) free(keys[i]);
            free(decrypted_text);
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

    for (int length_string = 0; length_string < secret_length; length_string+=BUFFER_LENGTH) {
        const size_t remaining_bytes = secret_length - length_string;
        const size_t bytes_to_copy = remaining_bytes < BUFFER_LENGTH ? remaining_bytes : BUFFER_LENGTH;

        memset(buffer, 0, BUFFER_LENGTH + 1);
        memcpy(buffer, secret_text + length_string, bytes_to_copy);

        unsigned char (*matrix)[4] = createMatrix(buffer, BUFFER_LENGTH);
        if (matrix == NULL)
            break;

        addRoundKey(matrix, keys[NUMBER_OF_ROUNDS]);

        for (int round = NUMBER_OF_ROUNDS - 1; round >= 1; round--) {
            inverseShiftRows(matrix);
            inverseSubBytes(matrix);
            addRoundKey(matrix, keys[round]);
            inverseMixColumns(matrix);
        }

        inverseShiftRows(matrix);
        inverseSubBytes(matrix);
        addRoundKey(matrix, keys[0]);

        char* chunk_string = printMatrix(matrix);
        if (chunk_string != NULL) {
            memcpy(decrypted_text + current_offset, chunk_string, BUFFER_LENGTH);
            current_offset += BUFFER_LENGTH;

            free(chunk_string);
        }

        free(matrix);
    }
    decrypted_text[total_decrypted_length] = '\0';

    for (int round = 0; round <= NUMBER_OF_ROUNDS; round++) {
        free(keys[round]);
        keys[round] = NULL;
    }

    return decrypted_text;
}
