#include "../include/keySchedule.h"
#include <stddef.h>

#define SIZE_MATRIX 4

void keySchedule(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    unsigned char old_matrix[SIZE_MATRIX][SIZE_MATRIX];

    for (int row = 0; row < SIZE_MATRIX; row++) {
        for (int col = 0; col < SIZE_MATRIX; col++) {
            old_matrix[row][col] = matrix[row][col];
        }
    }

    for (int col = 0; col < SIZE_MATRIX; col++) {
        for (int row = 0; row < SIZE_MATRIX; row++) {
            matrix[row][col] = matrix[row][(col + 3) % SIZE_MATRIX] ^ old_matrix[row][col];
        }
    }
}