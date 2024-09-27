#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

void aes_encript(uint8_t *state, uint8_t *key);

void exec_cypher(uint8_t* data, uint8_t* key, int pixels){
    const int block_size = 16;
    
    for(int i=0; i<(pixels*4); i=i+block_size){
        aes_encript(data+i, key);
    }
}

uint8_t GF_Mult(uint8_t a, uint8_t b);
/* Uso: ejecutable <input file (txt)> <output_file (txt)>*/
int main(int argc, char** argv){
    //printf("0x%08lx\n", GF_Mult(0x3, 0xff));
    //exit(0);
    if(argc < 3){
        fprintf(stderr, "No se tienen suficientes parámetros");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if(input == NULL){
        return 2;
    }
    FILE *output = fopen(argv[2], "w"); 
    if(output == NULL){
        return 2;
    }
    int rows;
    int cols;
    if(fscanf(input, "%d\n", &rows) ==EOF) return 3;
    if(fscanf(input, "%d\n", &cols) ==EOF) return 3;
    int pixel_count = rows*cols; 
    uint32_t* image = (uint32_t*) calloc(pixel_count, sizeof(uint32_t));

    if(image == NULL) return 3;
    int i = 0;
    char pixel[10];    
    uint32_t pixel_val;
    while(fscanf(input, "%s\n", &pixel)!=EOF){
        pixel_val = strtoul(pixel, NULL, 16);
        //printf("%08lx\n",pixel_val);
        *(image+i) = pixel_val;
        ++i;
    }
 
    // Los movimientos de memoria son parte del tradeoff. La carga del archivo a memoria de CPU no
    // cuenta, pero para ser justos, el movimiento de datos a memoria de GPU si se va a contar
    clock_t start = clock();
    

    uint8_t key[16] = {
        0xEF, 0xCD, 0xAB, 0x89,
        0x67, 0x45, 0x23, 0x01,
        0xEF, 0xCD, 0xAB, 0x89,
        0x67, 0x45, 0x23, 0x01,
        };
    exec_cypher((uint8_t*)image, key,pixel_count);
    
    clock_t end = clock();
    printf("Filtro tomó: %lf segundos\n", ((double)(end - start))/CLOCKS_PER_SEC);

    fprintf(output, "%d\n%d\n", rows, cols);
    for(int i=0; i < pixel_count; i++){
        fprintf(output, "%08lX\n", image[i]);
    }
    free(image);
}
