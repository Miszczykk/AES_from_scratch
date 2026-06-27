#include "../include/rCon.h"
#include <stdio.h>

#define SIZE_MATRIX 4
#define MAX_ROUND 10

unsigned char powerOfTwo[MAX_ROUND] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

void rCon(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], const int round) {
    if (matrix == NULL || round >= MAX_ROUND || round < 0)
        return;

    matrix[0][SIZE_MATRIX-1] = matrix[0][SIZE_MATRIX-1] ^ powerOfTwo[round];
}