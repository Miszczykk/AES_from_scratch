#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "rotWord.h"
#include "rCon.h"
#include "mixColumns.h"
#include "keySchedule.h"
#include "addRoundKey.h"

#define BUFFER_LENGTH 17 // 16 + 1
#define SOURCE_FILE_PATH "../PublicText.txt"
#define SECRET_KEY "../PublicText.txt"

int main(void) {
    FILE *file = fopen(SOURCE_FILE_PATH, "rb");
    char buffer[BUFFER_LENGTH];
    if (file != NULL) {
        unsigned char (*key)[4] = createMatrix(SECRET_KEY);
        printMatrix(key, "key");

        rotWord(key);
        printMatrix(key, "rotWord");

        subWord(key);
        printMatrix(key, "subWord");

        rCon(key, 0);
        printMatrix(key, "rCon for 0 round");

        keySchedule(key);
        printMatrix(key, "keySchedule");

        while(fgets(buffer, BUFFER_LENGTH, file)) {
            unsigned char (*matrix)[4] = createMatrix(buffer);

            if (matrix != NULL) {
                printMatrix(matrix, buffer);

                addRoundKey(matrix, key);
                printMatrix(matrix, "addRoundKey");

                subBytes(matrix);
                printMatrix(matrix, "subBytes");

                shiftRows(matrix);
                printMatrix(matrix, "shiftRows");

                mixColumns(matrix);
                printMatrix(matrix, "mixColumns");

                free(matrix);
            }
        }
        free(key);
        fclose(file);
    }
    return 0;
}