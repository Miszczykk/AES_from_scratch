#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "subBytes.h"
#include "shiftRows.h"
#include "rotWord.h"

#define BUFFER_LENGTH 17 // 16 + 1
#define SOURCE_FILE_PATH "../PublicText.txt"
#define SECRET_KEY "Ala Ma Kota Psie"

int main(void) {
    FILE *file = fopen(SOURCE_FILE_PATH, "rb");
    char buffer[BUFFER_LENGTH];
    if (file != NULL) {
        printf("\nkey:\n");
        unsigned char (*key)[4] = createMatrix(SECRET_KEY);
        printMatrix(key);

        printf("\nrotWord:\n");
        rotWord(key);
        printMatrix(key);

        while(fgets(buffer, BUFFER_LENGTH, file)) {
            printf("%s\n", buffer);
            unsigned char (*matrix)[4] = createMatrix(buffer);

            if (matrix != NULL) {
                printMatrix(matrix);
                    printf("\nsubBytes:\n");
                    subBytes(matrix);
                    printMatrix(matrix);

                    printf("\nshiftRows:\n");
                    shiftRows(matrix);
                    printMatrix(matrix);
                free(matrix);
            }
            printf("\n");
        }
        free(key);
        fclose(file);
    }
    return 0;
}