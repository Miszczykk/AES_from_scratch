#include "addRoundKey.h"
#include <stddef.h>

#define SIZE_MATRIX 4

void addRoundKey(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char key[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL || key == NULL)
        return;

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            matrix[row][col] = matrix[row][col] ^ key[row][col];
        }
    }
}