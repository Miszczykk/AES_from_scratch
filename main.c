#include "include/encrypt.h"

#define SOURCE_FILE_PATH "../PublicText.txt"
#define SECRET_KEY "../PublicText.txt"

int main(void) {
    encrypt(SOURCE_FILE_PATH, SECRET_KEY);
    return 0;
}