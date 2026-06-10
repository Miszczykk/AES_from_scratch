#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#define BUFFER_LENGTH 17 // 16 + 1
#define SOURCE_FILE_PATH "../PublicText.txt"

int main(void) {
    FILE *file = fopen(SOURCE_FILE_PATH, "rb");
    char buffer[BUFFER_LENGTH];
    if (file != NULL) {
        while(fgets(buffer, BUFFER_LENGTH, file)) {
            printf("%s\n", buffer);
            unsigned char (*matrix)[4] = createMatrix(buffer);
            if (matrix != NULL) {
                printMatrix(matrix);
                free(matrix);
            }
            printf("\n");
        }
        fclose(file);
    }
    return 0;
}