#include "../include/rotWord.h"
#include <stdio.h>

#define SIZE_MATRIX 4
#define LAST_COL (SIZE_MATRIX-1)

void reverseCol(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], int top, int bottom) {

    while (top < bottom) {
        const unsigned char temp = matrix[top][LAST_COL];
        matrix[top][LAST_COL] = matrix[bottom][LAST_COL];
        matrix[bottom][LAST_COL] = temp;

        top++;
        bottom--;
    }

}

void rotWord(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]) {
    if (matrix == NULL)
        return;

    reverseCol(matrix, 1, SIZE_MATRIX-1);
    reverseCol(matrix, 0, SIZE_MATRIX-1);
}