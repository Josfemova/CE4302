#include <stdio.h>
#include <stdint.h>
#include <string.h>


// Rcon (Constantes de ronda)
static const uint8_t rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10,
    0x20, 0x40, 0x80, 0x1B, 0x36
};

static const uint8_t sbox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

static const uint8_t inv_sbox[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

//________________/ Print State /__________________________________________

void printState(uint8_t *state){
    for (int i = 0; i < 16; i++) {
        printf("%02x ", state[i]);
    }
    printf("\n");

}

//________________/ Add Round Key /__________________________________________

// state: 16 bytes, roundKey: 16 bytes
void addRoundKey(uint8_t *state, const uint8_t *roundKey) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= roundKey[i]; // XOR
        // modifica state directamente 
    }
}

//________________/ Sub Bytes /__________________________________________

void subBytes(uint8_t *state) {
    for (int i = 0; i < 16; i++) {
        state[i] = sbox[state[i]]; // sustitucion con la Sbox
    }
}

void invSubBytes(uint8_t *state) {
    for (int i = 0; i < 16; i++) {
        state[i] = inv_sbox[state[i]]; //sustitucion con la Sbox inversa
    }
}

//________________/ Shift Rows /__________________________________________

void shiftRows(uint8_t *state) {
    uint8_t temp[16];

    // Columna 1
    temp[0]  = state[0]; //Primera fila (sin cambios) 
    temp[1]  = state[5]; // Segunda fila: rotar 1 byte a la izquierda
    temp[2]  = state[10]; // Tercera fila: rotar 2 bytes a la izquierda
    temp[3]  = state[15]; // Cuarta fila: rotar 3 bytes a la izquierda

    // Columna 2
    temp[4]  = state[4];
    temp[5]  = state[9];
    temp[6]  = state[14];
    temp[7]  = state[3];

    // Columna 3
    temp[8]  = state[8];
    temp[9]  = state[13];
    temp[10] = state[2];
    temp[11] = state[7];

    // Columna 4
    temp[12] = state[12];
    temp[13] = state[1];
    temp[14] = state[6];
    temp[15] = state[11];

    for (int i = 0; i < 16; i++) {
        state[i] = temp[i];
    }
}


void invShiftRows(uint8_t *state) {
    uint8_t temp[16];

    // Columna 1
    temp[0]  = state[0]; 
    temp[1]  = state[13]; // Tercera fila: rotar 1 byte a la derecha
    temp[2]  = state[10]; // Segunda fila: rotar 2 bytes a la derecha
    temp[3]  = state[7];  // Primera fila: rotar 3 bytes a la derecha

    // Columna 2
    temp[4]  = state[4];
    temp[5]  = state[1];
    temp[6]  = state[14];
    temp[7]  = state[11];

    // Columna 3
    temp[8]  = state[8];
    temp[9]  = state[5];
    temp[10] = state[2];
    temp[11] = state[15];

    // Columna 4
    temp[12] = state[12];
    temp[13] = state[9];
    temp[14] = state[6];
    temp[15] = state[3];

    memcpy(state, temp, 16);
}



//________________/ Mix Columns /__________________________________________

uint8_t GF_Mult(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    uint8_t temp = a;

    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            result ^= temp;  // Sumar a result si el bit menos significativo de b es 1
        }
        uint8_t high_bit_set = temp & 0x80;  // Verificar si el bit mas alto esta encendido
        temp <<= 1;  // Desplazar a la izquierda (multiplicar por 2)
        if (high_bit_set) {
            temp ^= 0x1B;  // Reducir si es necesario con el polinomio irreducible
        }
        b >>= 1;  // Desplazar a la derecha para procesar el siguiente bit de b
    }

    return result;
}

void mixColumns(uint8_t *state) {
    uint8_t temp[16];  // Arreglo temporal para almacenar el resultado

    for (int j = 0; j < 4; j++) {  // Para cada columna
        temp[j * 4 + 0] = GF_Mult(0x02, state[j * 4 + 0]) ^ GF_Mult(0x03, state[j * 4 + 1]) ^ state[j * 4 + 2] ^ state[j * 4 + 3];
        temp[j * 4 + 1] = state[j * 4 + 0] ^ GF_Mult(0x02, state[j * 4 + 1]) ^ GF_Mult(0x03, state[j * 4 + 2]) ^ state[j * 4 + 3];
        temp[j * 4 + 2] = state[j * 4 + 0] ^ state[j * 4 + 1] ^ GF_Mult(0x02, state[j * 4 + 2]) ^ GF_Mult(0x03, state[j * 4 + 3]);
        temp[j * 4 + 3] = GF_Mult(0x03, state[j * 4 + 0]) ^ state[j * 4 + 1] ^ state[j * 4 + 2] ^ GF_Mult(0x02, state[j * 4 + 3]);
    }

    // Copiar el resultado de temp al estado original
    for (int i = 0; i < 16; i++) {
        state[i] = temp[i];
    }
}

void invMixColumns(uint8_t *state) {
    uint8_t temp[16];  

    for (int j = 0; j < 4; j++) {  // Para cada columna
        temp[j * 4 + 0] = GF_Mult(0x0E, state[j * 4 + 0]) ^ GF_Mult(0x0B, state[j * 4 + 1]) ^ GF_Mult(0x0D, state[j * 4 + 2]) ^ GF_Mult(0x09, state[j * 4 + 3]);
        temp[j * 4 + 1] = GF_Mult(0x09, state[j * 4 + 0]) ^ GF_Mult(0x0E, state[j * 4 + 1]) ^ GF_Mult(0x0B, state[j * 4 + 2]) ^ GF_Mult(0x0D, state[j * 4 + 3]);
        temp[j * 4 + 2] = GF_Mult(0x0D, state[j * 4 + 0]) ^ GF_Mult(0x09, state[j * 4 + 1]) ^ GF_Mult(0x0E, state[j * 4 + 2]) ^ GF_Mult(0x0B, state[j * 4 + 3]);
        temp[j * 4 + 3] = GF_Mult(0x0B, state[j * 4 + 0]) ^ GF_Mult(0x0D, state[j * 4 + 1]) ^ GF_Mult(0x09, state[j * 4 + 2]) ^ GF_Mult(0x0E, state[j * 4 + 3]);
    }

    // Copiar el resultado de temp al estado original
    for (int i = 0; i < 16; i++) {
        state[i] = temp[i];
    }
}

//____________________/ Key Expansion /_____________________________________

// Rotacion de palabras
void rotWord(uint8_t *word) {
    uint8_t saved[4];
    memcpy(saved, word, 4);
    word[0] = saved[1];
    word[1] = saved[2];
    word[2] = saved[3];
    word[3] = saved[0];
}

// Sustitucion de bytes usando la S-box
void subWord(uint8_t *word) {
    for (int i = 0; i < 4; i++) {
        word[i] = sbox[word[i]];
    }
}

// Expansion de clave
void keyExpansion(const uint8_t *key, uint8_t *expandedKeys) {
    // Inicializar las primeras palabras con la clave original
    memset(expandedKeys, 0, 16*11);
    memcpy(expandedKeys, key, 16);
    // Generar el resto de las claves
    uint8_t w0[4];
    uint8_t w1[4];
    uint8_t w2[4];
    uint8_t w3[4];

    uint8_t transformed[4];
    for(int i=0; i< 10; i++){
        uint8_t* base_last_key = expandedKeys + (16*i);
        uint8_t* base_next_key = expandedKeys + (16*(i+1));
        memcpy(w0,base_last_key + 0 , 4);
        memcpy(w1,base_last_key + 4 , 4);
        memcpy(w2,base_last_key + 8 , 4);
        memcpy(w3,base_last_key + 12, 4);
        memcpy(transformed, w3, 4);
        rotWord(transformed);
        subWord(transformed);

        transformed[0] ^= rcon[i]; // byte más bajo
        uint32_t trans = *((uint32_t*)transformed);
        uint32_t w20 = *((uint32_t*)w0);

        *((uint32_t*)w0) ^= *((uint32_t*)transformed); 
        *((uint32_t*)w1) ^= *((uint32_t*)w0); 
        *((uint32_t*)w2) ^= *((uint32_t*)w1); 
        *((uint32_t*)w3) ^= *((uint32_t*)w2); 
        // guardar 
        memcpy(base_next_key + 0 ,w0, 4);                
        memcpy(base_next_key + 4 ,w1, 4);                
        memcpy(base_next_key + 8 ,w2, 4);                
        memcpy(base_next_key + 12,w3, 4); 
    }           
}

void printExpandedKeys(const uint8_t *expandedKeys) {
    for (int i = 0; i < 11; i++) {
        printf("Clave %02d: ", i);
        for (int j = 0; j < 16; j++) {
            printf("%02x ", (uint8_t) expandedKeys[i * 16 + j]);
        }
        printf("\n");
    }
}

//_______________________/ AES ENCRYPT /_______________________________________

void aes_encript(uint8_t *state, uint8_t *key) {
    uint8_t roundKeys[176];  // Almacenar 11 claves de ronda (11 * 16 = 176 bytes)
    keyExpansion(key, roundKeys);  // expand a la key original
    // Ronda inicial: AddRoundKey
    addRoundKey(state, roundKeys); 
    
    // 9 rondas de AES 
    for (int round = 1; round <= 9; round++) {
        subBytes(state);                // Sustituye los bytes usando S-box
        shiftRows(state);               // Rotacion de filas
        mixColumns(state);              // Mezcla de columnas
        addRoundKey(state, roundKeys + (round * 16));  // Agregar clave de ronda
    }

    // Ronda final (sin mixColumns)
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKeys + (10 * 16));  // Usar la última clave de ronda
    
}

//_________________________/ AES DECRYPT /______________________________________

void aes_decrypt(uint8_t* state, const uint8_t* key) { //El state de entrada debe ser el encriptado
    uint8_t stateTemp[16];

    uint8_t roundKeys[176];  // Almacenar 11 claves de ronda (11 * 16 = 176 bytes)
    keyExpansion(key, roundKeys);  // Expand a la key original: mismas claves que para el encrypt

    // Copiar el bloque del cifrado
    memcpy(stateTemp, state, 16);

    // Primera ronda: AddRoundKey con la ultima clave expandida
    addRoundKey(stateTemp, roundKeys + 160); // Clave 10: 160+16=176

    // Nueve rondas: del 9 al 1
    for (int round = 9; round > 0; round--) {
        invShiftRows(stateTemp);  
        invSubBytes(stateTemp);
        addRoundKey(stateTemp, roundKeys + (round * 16));
        invMixColumns(stateTemp);
    }

    // Ultima ronda (ronda 0)
    invShiftRows(stateTemp);
    invSubBytes(stateTemp);
    addRoundKey(stateTemp, roundKeys); // Clave original

    // Copiar el estado descifrado al bloque de salida
    memcpy(state, stateTemp, 16);
}



