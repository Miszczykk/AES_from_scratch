#include "../include/matrix.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE_MATRIX 4

unsigned char (*createMatrix(const char* buffer, size_t bytes_read))[SIZE_MATRIX] {
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

void printMatrix(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], const char* source_to_file) {
    if (matrix == NULL)
        return;

    char *string = malloc((SIZE_MATRIX * SIZE_MATRIX + 1) * sizeof(char));

    if (string == NULL) {
        free(string);
        return;
    }
    int index = 0;

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            printf("%02x", matrix[row][col]);
            string[index] = (char)matrix[row][col];
            index++;
        }
    }
    string[index] = '\0';

    FILE *file = fopen(source_to_file, "a");
    if (file != NULL) {
        fprintf(file, string);
    }
    fclose(file);
    free(string);
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