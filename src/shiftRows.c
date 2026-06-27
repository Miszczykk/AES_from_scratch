#include "../include/shiftRows.h"
#include <stdio.h>

#define SIZE_MATRIX 4

void reverse(unsigned char row[SIZE_MATRIX], int left, int right) {
    while (left < right) {
        const unsigned char temp = row[left];
        row[left] = row[right];
        row[right] = temp;

        left++;
        right--;
    }
}

void shiftRows(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    for (int row = 1; row < SIZE_MATRIX; row++) {
        reverse(matrix[row], 0, row-1);
        reverse(matrix[row], row, SIZE_MATRIX-1);
        reverse(matrix[row], 0, SIZE_MATRIX-1);
    }
}