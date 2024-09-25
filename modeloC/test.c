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
        0x30, 0xeb, 0x06, 0x98,
        0xe1, 0x49, 0xbf, 0xa0,
        0x20, 0xa9, 0x32, 0x79,
        0x31, 0xb5, 0x53, 0x52
    };

    printf("AES Decrypt: ");
    aes_decrypt(encrypted, key);
    printState(encrypted); // Encrypted ahora guarda el resultado

    return 0;
}