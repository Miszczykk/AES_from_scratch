#ifndef AES_FROM_SCRATCH_MATRIX_H
#define AES_FROM_SCRATCH_MATRIX_H
#define SIZE_MATRIX 4
unsigned char (*createMatrix(const char* buffer))[SIZE_MATRIX];
void printMatrix(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], char* title);
void copyMatrix(unsigned char copy_matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char original_matrix[SIZE_MATRIX][SIZE_MATRIX]);
#endif