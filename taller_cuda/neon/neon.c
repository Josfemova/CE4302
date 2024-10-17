#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <arm_neon.h>
#include <time.h>
#include <math.h>

int16x4_t op_x[3] = {
    {-1, 0, 1, 0},
    {-2, 0, 2, 0},
    {-1 ,0, 1, 0}  
}; 

int16x4_t op_y[3] = {
    {-1,-2,-1, 0},
    { 0, 0, 0, 0},
    { 1, 2, 1, 0}  
}; 

int filtro_pixel(int16_t* matriz_entrada, int row, int col, int cols, int16x4_t op[3]){
    int curr = row*cols + col - 1;
    int16x4_t row0 = vld1_s16(matriz_entrada + curr - cols);
    int16x4_t row1 = vld1_s16(matriz_entrada + curr);
    int16x4_t row2 = vld1_s16(matriz_entrada + curr + cols); 
    
    int16x4_t vres = {0,0,0,0};

    vres = vmla_s16(vres, row0, op[0]);
    vres = vmla_s16(vres, row1, op[1]);
    vres = vmla_s16(vres, row2, op[2]);
    return vaddv_s16(vres);
}


// Recibe la imagen con padding, y la guarda con padding 
void filtro(int16_t* matriz_entrada, int rows, int cols, int16_t* matriz_salida){
    // comenzar en x=1, y = 1 
    // cargar vector en x-1, y-1 
    // cargar vector en x-1, y 
    // cargar vector en x-1, y+1
    int result=0;
    for(int row=1; row<(rows-1);row++){
        for(int col=1; col< (cols-1); col++){
            int valx = filtro_pixel(matriz_entrada, row, col,cols, op_x);
            int valy = filtro_pixel(matriz_entrada, row, col,cols, op_y);
            result = (int)(sqrt(valx*valx + valy*valy));
            if(result > 255) result = 255;
            *(matriz_salida + row*cols+col) = result;
        }
    }
}


int main(int argc, char** argv){
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
    if(fscanf(input, "%d\n", &rows)==EOF) return 3;
    if(fscanf(input, "%d\n", &cols) ==EOF) return 3;
    int pixel_count = (rows)*(cols); //aplicar padding 

    // reservar memoria inicializada en 0

    // 1 pixel de mas en source para el filtro en última esquina
    int16_t* src_image = calloc((pixel_count+1), sizeof(int16_t));
    int16_t* out_image = calloc(pixel_count, sizeof(int16_t));

    if(src_image == NULL || out_image == NULL) return 3;
    int i = 0;
    int pixel;
    
    while(fscanf(input, "%d\n", &pixel)!=EOF){
        src_image[i] = pixel;
        ++i;
    }

    clock_t start = clock();

    filtro(src_image, rows, cols, out_image);

    clock_t end = clock();

    printf("Filtro tomó: %lf segundos\n", ((double)(end - start))/CLOCKS_PER_SEC);
    
    fprintf(output, "%d\n%d\n", rows, cols);
    for(int i=0; i < pixel_count; i++){
        fprintf(output, "%d\n", *(out_image+i));
    }

    free(src_image);
    free(out_image);
}
