#include "../include/matrix.h"

#include <stdlib.h>

#define SIZE_MATRIX 4

unsigned char (*createMatrix(const char* buffer, const size_t bytes_read))[SIZE_MATRIX] {
    //row x col
    unsigned char (*matrix)[SIZE_MATRIX] = malloc(SIZE_MATRIX * SIZE_MATRIX * sizeof(unsigned char));

    if (matrix == NULL)
        return NULL;

    int position = 0;

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            if (position < bytes_read) {
                matrix[row][col] = buffer[position];
                position++;
            }else {
                matrix[row][col] = 0x00;
            }
        }
    }
    return matrix;
}

char* printMatrix(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return NULL;

    char *string = malloc((SIZE_MATRIX * SIZE_MATRIX + 1) * sizeof(char));

    if (string == NULL) {
        free(string);
        return NULL;
    }
    int index = 0;

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            string[index] = (char)matrix[row][col];
            index++;
        }
    }
    string[index] = '\0';

    return string;
}

void copyMatrix(unsigned char copy_matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char original_matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (copy_matrix == NULL || original_matrix == NULL)
        return;

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            copy_matrix[row][col] = original_matrix[row][col];
        }
    }
}