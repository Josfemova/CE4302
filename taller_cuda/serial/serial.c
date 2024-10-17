#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define WIDTH  256 // Ancho de la imagen
#define HEIGHT 256  // Alto de la imagen

// Sobel kernels para direccion X y Y
int sobel_x[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

int sobel_y[3][3] = {
    {-1, -2, -1},
    { 0,  0,  0},
    { 1,  2,  1}
};


// Aplicar el filtro Sobel a una imagen (escala de grises)
void edgeDetectionSerial(int16_t* input, int rows, int cols, int16_t* output) {
    int gx, gy;

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            gx = 0;
            gy = 0;

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    int input_index = (i + x) * cols + (j + y);
                    gx += input[input_index] * sobel_x[x + 1][y + 1];
                    gy += input[input_index] * sobel_y[x + 1][y + 1];
                }
            }

            int magnitude = (int)sqrt(gx * gx + gy * gy);

            if (magnitude > 255) {
                magnitude = 255;
            }

            output[i * cols + j] = (int16_t)magnitude;
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
    
    int pixel_count = rows * cols * 3;  // Multiplicamos por 3 para R, G, B
    int16_t* src_image = (int16_t*) malloc(pixel_count * sizeof(int16_t));
    int16_t* out_image = (int16_t*) malloc(pixel_count * sizeof(int16_t));
    if(src_image == NULL || out_image == NULL) return 3;
    
    int i = 0;
    int pixel;
    
    while(fscanf(input, "%d\n", &pixel) != EOF){
        src_image[i] = pixel;
        ++i;
    }
    
    clock_t start = clock();

    edgeDetectionSerial(src_image, rows, cols, out_image);

    clock_t end = clock();

    printf("Filtro tomó: %lf segundos\n", ((double)(end - start))/CLOCKS_PER_SEC);
    
    fprintf(output, "%d\n%d\n", rows, cols);
    for(int i = 0; i < pixel_count; i++){
        fprintf(output, "%d\n", out_image[i]);
    }

    free(src_image);
    free(out_image);
    fclose(input);
    fclose(output);

    return 0;
}
