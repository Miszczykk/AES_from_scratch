#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE_MATRIX 4

unsigned char (*createMatrix(const char* buffer))[SIZE_MATRIX] {
    //row x col
    unsigned char (*matrix)[SIZE_MATRIX] = malloc(SIZE_MATRIX * SIZE_MATRIX * sizeof(unsigned char));

    if (matrix == NULL)
        return NULL;

    int position = 0;

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            if (buffer[position] != '\0') {
                matrix[row][col] = buffer[position];
                position++;
            }else {
                matrix[row][col] = 0x00;
            }
        }
    }
    return matrix;
}

void printMatrix(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], char* title) {
    if (matrix == NULL)
        return;

    printf("\n%s\n", title);

    for (int row = 0; row < SIZE_MATRIX; row++) {
        printf("[ ");
        for (int col = 0; col < SIZE_MATRIX; col++) {
            printf("%02X ", matrix[row][col]);
        }
        printf("]\n");
    }
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