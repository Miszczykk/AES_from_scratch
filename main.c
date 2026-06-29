#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/decryption.h"
#include "include/encrypt.h"

#define PUBLIC_TEXT argv[1]
#define SECRET_KEY argv[2]
#define BUFFER_LENGTH 16

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <PUBLIC_TEXT> <SECRET_KEY>\n", argv[0]);
    }

    const int text_length = strlen(PUBLIC_TEXT);
    int num_blocks = (text_length / BUFFER_LENGTH) + (text_length % BUFFER_LENGTH != 0 ? 1 : 0);
    if (num_blocks == 0) num_blocks = 1;
    const int total_length = num_blocks * BUFFER_LENGTH;

    char* encrypt_text = encrypt(PUBLIC_TEXT, text_length, SECRET_KEY);

    if (encrypt_text != NULL) {
        for (size_t i = 0; i < total_length; i++) {
            printf("%02x", (unsigned char)encrypt_text[i]);
        }
        printf("\n");
    }

    char* decrypt_text = decryption(encrypt_text, total_length ,SECRET_KEY);

    if (decrypt_text != NULL) {
        for (size_t i = 0; i < text_length; i++) {
            printf("%c", decrypt_text[i]);
        }
        printf("\n");
        free(decrypt_text);
    }

    free(encrypt_text);
    return 0;
}