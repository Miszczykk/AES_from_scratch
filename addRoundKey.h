#ifndef AES_FROM_SCRATCH_ADDROUNDKEY_H
#define AES_FROM_SCRATCH_ADDROUNDKEY_H
#define SIZE_MATRIX 4
void addRoundKey(unsigned char matrix[SIZE_MATRIX][SIZE_MATRIX], unsigned char key[SIZE_MATRIX][SIZE_MATRIX]);
#endif