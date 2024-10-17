#include "aes.c"
#include <stdio.h>
#include <unistd.h>


int main() {
    // Valor de prueba para state

    uint8_t state[16] = {
        0x87, 0x6e, 0x46, 0xa6, //columna 1
        0xf2, 0x4c, 0xe7, 0x8c, //columna 2
        0x4d, 0x90, 0x4a, 0xd8, //columna 3
        0xe7, 0xec, 0xc3, 0x95  //columna 4
    };

    // Valor de prueba para key
    uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x8c, 0xc9,
        0x7a, 0x24, 0x76, 0x0e
    };

    printf("State Original: ");
    printState(state);

    printf("Key: ");
    printState(key);

    printf("AES Encrypt: ");
    aes_encript(state, key);
    printState(state);

    // Este deberia ser el resultado del encpryt de state
    uint8_t encrypted[16] = {
        0x4d, 0x39, 0x3e, 0x54,
        0x14, 0x75, 0x94, 0x88,
        0x80, 0x9e, 0xae, 0x7c,
        0x60, 0x7c, 0x08, 0x5b
    };

    printf("AES Decrypt: ");
    aes_decrypt(encrypted, key);
    printState(encrypted); // Encrypted ahora guarda el resultado

    return 0;
}