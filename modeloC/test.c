#include "aes.c"
#include <stdio.h>
#include <unistd.h>

int main() {
    // Valor de prueba para state
    /**uint8_t state[16] = {
        0x32, 0x88, 0x31, 0xe0,
        0x43, 0x5a, 0x31, 0x37,
        0xf6, 0x30, 0x98, 0x07,
        0xa8, 0x8d, 0xa1, 0x2c
    };**/

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

    /**printf("Add Round Key: ");
    addRoundKey(state, key);
    printState(state);

    printf("Sub Bytes: ");
    subBytes(state);
    printState(state);

    printf("Inv Sub Bytes: ");
    invSubBytes(state);
    printState(state);

    printf("Shift Rows: ");
    shiftRows(state);
    printState(state);

    printf("Mix Columns: ");
    mixColumns(state);
    printState(state);

    printf("Key Expansion: ");
    uint8_t roundKeys[176];
    keyExpansion(key, roundKeys);
    printExpandedKeys(roundKeys, 44);**/ 


    printf("AES Encrypt: ");
    aes_encript(state, key);
    printState(state);

    return 0;
}