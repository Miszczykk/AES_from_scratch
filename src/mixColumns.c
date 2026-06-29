#include "../include/mixColumns.h"

#include <stdlib.h>

#define SIZE_MATRIX 4

static unsigned char state_matrix[SIZE_MATRIX][SIZE_MATRIX] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02},
};

static unsigned char inverse_state_matrix[SIZE_MATRIX][SIZE_MATRIX] = {
    {0x0E, 0x0B, 0x0D, 0x09},
    {0x09, 0x0E, 0x0B, 0x0D},
    {0x0D, 0x09, 0x0E, 0x0B},
    {0x0B, 0x0D, 0x09, 0x0E}
};

static unsigned char galoisFieldMultiplication(unsigned char a, unsigned char b) {
    unsigned char result = 0;

    for (int bit = 0; bit < 8; bit++) {
        if ((b & 1) != 0)
            result ^= a;

        unsigned char high_bit_set = (a & 0x80);
        a <<= 1;

        if (high_bit_set != 0)
            a ^= 0x1B;

        b >>= 1;
    }

    return result;
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

void inverseMixColumns(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    unsigned char new_matrix[SIZE_MATRIX][SIZE_MATRIX] = {0};

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            for (int index = 0; index < SIZE_MATRIX; index++) {
                new_matrix[row][col] ^= galoisFieldMultiplication(inverse_state_matrix[row][index], matrix[index][col]);
            }
        }
    }

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            matrix[row][col] = new_matrix[row][col];
        }
    }
}