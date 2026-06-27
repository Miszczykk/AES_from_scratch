#include "../include/mixColumns.h"

#include <stdlib.h>

#define SIZE_MATRIX 4

unsigned char state_matrix[SIZE_MATRIX][SIZE_MATRIX] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02},
};

unsigned char galoisFieldMultiplication(unsigned char a, unsigned char b) {
    switch (a) {
        case 0x01:
            return b;
        case 0x02:
            return b << 1 ^ (b & 0x80 ? 0x1B : 0x00);
        case 0x03:
            return b << 1 ^ (b & 0x80 ? 0x1B : 0x00) ^ b;
        default:
            return 0;
    }
}

void mixColumns(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    unsigned char new_matrix[SIZE_MATRIX][SIZE_MATRIX] = {0};

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            for (int index = 0; index < SIZE_MATRIX; index++) {
                new_matrix[row][col] ^= galoisFieldMultiplication(state_matrix[row][index], matrix[index][col]);
            }
        }
    }

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            matrix[row][col] = new_matrix[row][col];
        }
    }
}