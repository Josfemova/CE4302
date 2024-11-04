#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>

// Sobel kernels para direccion X y Y
int16_t sobel_x[9] = {
    -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1};

int16_t sobel_y[9] = {
    -1, -2, -1,
    0, 0, 0,
    1, 2, 1};

void filtro_pixel(int16_t *matriz_entrada, int rows, int cols, int row, int col, int16_t *matriz_salida)
{
    int16_t *opx = sobel_x;
    int16_t *opy = sobel_y;

    // curr = row*col - 1
    // prev = (row-1)*col - 1
    // post = (row+1)*col - 1
    int curr = row * cols + col - 1;
    if (col == 0 || col == (cols - 1) || row == 0 || row == (rows - 1))
    {
        // no procesar bordes
        *(matriz_salida + curr + 1) = *(matriz_entrada + curr + 1);
        return;
    }
    int16_t *prow0 = matriz_entrada + curr - cols;
    int16_t *prow1 = matriz_entrada + curr;
    int16_t *prow2 = matriz_entrada + curr + cols;
    int16_t result;
    int resultx = 0;
    int resulty = 0;
    // opx y opy tienen que se copiadas a mem de la gpu duuuhh

    for (int i = 0; i < 3; i++)
    {
        resultx += *(prow0 + i) * opx[0 + i];
        resultx += *(prow1 + i) * opx[3 + i];
        resultx += *(prow2 + i) * opx[6 + i];
        resulty += *(prow0 + i) * opy[0 + i];
        resulty += *(prow1 + i) * opy[3 + i];
        resulty += *(prow2 + i) * opy[6 + i];
    }
    result = (int16_t)(sqrt((double)(resultx * resultx + resulty * resulty)));
    if (result > 255)
        result = 255;
    matriz_salida[curr + 1] = (result);
}

// Aplicar el filtro Sobel a una imagen (escala de grises)
void filtro(int16_t *input, int rows, int cols, int16_t *output)
{
    int row, col;
#pragma omp parallel for private(row, col)
    for (row = 1; row < rows - 1; row++)
    {
        for (col = 1; col < cols - 1; col++)
        {
            filtro_pixel(input, rows, cols, row, col, output);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "No se tienen suficientes parámetros");
        return 1;
    }
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        return 2;
    }
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        return 2;
    }
    int rows;
    int cols;

    if (fscanf(input, "%d\n", &rows) == EOF)
        return 3;
    if (fscanf(input, "%d\n", &cols) == EOF)
        return 3;

    int pixel_count = rows * cols; // Multiplicamos por 3 para R, G, B
    int16_t *src_image = (int16_t *)malloc(pixel_count * sizeof(int16_t));
    int16_t *out_image = (int16_t *)malloc(pixel_count * sizeof(int16_t));
    if (src_image == NULL || out_image == NULL)
        return 3;

    int i = 0;
    int pixel;

    while (fscanf(input, "%d\n", &pixel) != EOF)
    {
        src_image[i] = pixel;
        ++i;
    }

    clock_t start = clock();

    filtro(src_image, rows, cols, out_image);

    clock_t end = clock();

    printf("Filtro tomó: %lf segundos\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    fprintf(output, "%d\n%d\n", rows, cols);
    for (int i = 0; i < pixel_count; i++)
    {
        fprintf(output, "%d\n", out_image[i]);
    }

    free(src_image);
    free(out_image);
    fclose(input);
    fclose(output);

    return 0;
}
