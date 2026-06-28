#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/encrypt.h"

#define PUBLIC_TEXT "PUBLIC_TEXT123"
#define SECRET_KEY "SEKRETNY_KLUCZ12"
#define BUFFER_LENGTH 16

int main(void) {
    char* encrypt_text = encrypt(PUBLIC_TEXT, SECRET_KEY);

    if (encrypt_text != NULL) {
        const size_t text_length = strlen(PUBLIC_TEXT);
        size_t num_blocks = (text_length / BUFFER_LENGTH) + (text_length % BUFFER_LENGTH != 0 ? 1 : 0);
        if (num_blocks == 0) num_blocks = 1;
        const size_t total_encrypted_length = num_blocks * BUFFER_LENGTH;

        for (size_t i = 0; i < total_encrypted_length; i++) {
            printf("%02x", (unsigned char)encrypt_text[i]);
        }
        free(encrypt_text);
    }
    return 0;
}