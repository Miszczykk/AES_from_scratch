#ifndef AES_FROM_SCRATCH_MATRIX_H
#define AES_FROM_SCRATCH_MATRIX_H
#define SIZE_MATRIX 4
#include <stddef.h>
unsigned char (*createMatrix(const char* buffer, size_t bytes_read ))[SIZE_MATRIX];
char* printMatrix(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX]);
void copyMatrix(unsigned char copy_matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char original_matrix[SIZE_MATRIX][SIZE_MATRIX]);
#endif