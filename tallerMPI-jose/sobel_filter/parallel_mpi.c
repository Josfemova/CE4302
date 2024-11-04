#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Sobel kernels para direccion X y Y
int16_t sobel_x[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

int16_t sobel_y[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

void filtro_pixel(int16_t *input, int rows, int cols, int row,
                  int col, int16_t *output)
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
        *(output + curr + 1) = *(input + curr + 1);
        return;
    }
    int16_t *prow0 = input + curr - cols;
    int16_t *prow1 = input + curr;
    int16_t *prow2 = input + curr + cols;
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
    output[curr + 1] = (result);
}

int myrank, nprocs;
#define rows_cols_tag 2000
#define input_tag 2001
#define output_tag 2002

// Aplicar el filtro Sobel a una imagen (escala de grises)
void filtro_master(int16_t *input, int rows, int cols, int16_t *output)
{
    // Brainstorming: En este punto necesito distribuir la imagen
    // En un escenario normal, este suele ser el punto de partida. La imagen no
    // va a estar en los workers, entonces, cómo se distribuye?
    // R/ Se tiene que dividir la imagen en frames según la cantidad de workers,

    // R2/ Actually, frames no promete, es mejor enviar "franjas". Cada franja
    // contiene al menos su row anterior y siguiente. (Si no, se veria rallado)
    // Luego de enviar todas las franjas, estas se deben recibir sin las rows
    // anteriores y siguientes.

    MPI_Status status;
    int rows_cols[2] = {rows, cols};
    int workers = nprocs - 1;
    // se le quitan 2 rows por el row final y el inicial, se
    // quita 1 proc porque el master no va a trabajar.
    int rows_per_proc = (rows - 2) / (workers);
    int vals_per_row = cols;
    int send_vals_per_proc = (rows_per_proc + 2) * vals_per_row; 
    int recv_vals_per_proc = (rows_per_proc)*vals_per_row;       

    int missing_rows = (rows - 2) % workers;
    int send_vals_per_proc_last = send_vals_per_proc + missing_rows * vals_per_row;
    int recv_vals_per_proc_last = recv_vals_per_proc + missing_rows * vals_per_row;

    // Implementando como caso AMP, master es una compu normal y el cluster es
    // de puro number crunching
    int16_t *send_data_s = input;
    for (int i = 1; i < (nprocs - 1); i++)
    {
        // calcular el inicio de los datos a enviar
        MPI_Send(rows_cols, 2, MPI_INT, i, rows_cols_tag, MPI_COMM_WORLD);
        MPI_Send(send_data_s, send_vals_per_proc, MPI_INT16_T, i, input_tag, MPI_COMM_WORLD);
        send_data_s += recv_vals_per_proc;
    }
    // caso especial de último worker
    {
        MPI_Send(rows_cols, 2, MPI_INT, nprocs - 1, rows_cols_tag, MPI_COMM_WORLD);
        MPI_Send(send_data_s, send_vals_per_proc_last, MPI_INT16_T, nprocs - 1, input_tag, MPI_COMM_WORLD);
    }

    // Hora de recibir los resultados!
    int16_t *recv_data_s = output+vals_per_row;
    for (int i = 1; i < (nprocs - 1); i++)
    {
        MPI_Recv(recv_data_s, recv_vals_per_proc, MPI_INT16_T, i, output_tag, MPI_COMM_WORLD, &status);
        recv_data_s += recv_vals_per_proc;
    }
    // caso especial ultimo worker
    {
        MPI_Recv(recv_data_s, recv_vals_per_proc_last, MPI_INT16_T, nprocs - 1, output_tag, MPI_COMM_WORLD, &status);
    }
}

void filtro_worker()
{
    // Recibir Rows, Cols,
    MPI_Status status;
    int rows_cols[2];
    MPI_Recv(rows_cols, 2, MPI_INT, 0, rows_cols_tag, MPI_COMM_WORLD, &status);
    int rows = rows_cols[0];
    int cols = rows_cols[1];

    int workers = nprocs - 1;
    // se le quitan 2 rows por el row final y el inicial, se
    // quita 1 proc porque el master no va a trabajar.
    int rows_per_proc = (rows - 2) / (workers);
    int vals_per_row = cols;
    int send_vals_per_proc = (rows_per_proc + 2) * vals_per_row; // Multiplicamos por 3 para R, G, B
    int recv_vals_per_proc = (rows_per_proc)*vals_per_row;       // Multiplicamos por 3 para R, G, B
    int missing_rows = (rows - 2) % workers;

    int send_vals_per_proc_last = send_vals_per_proc + missing_rows * vals_per_row;
    int recv_vals_per_proc_last = recv_vals_per_proc + missing_rows * vals_per_row;

    // se agregan 2 filas, la anterior y la siguiente.
    int in_pixel_count = (myrank == (nprocs - 1)) ? send_vals_per_proc_last : send_vals_per_proc;
    int out_pixel_count = (myrank == (nprocs - 1)) ? recv_vals_per_proc_last : recv_vals_per_proc;

    int16_t *input_data = malloc(sizeof(int16_t) * in_pixel_count);
    int16_t *output_data = malloc(sizeof(int16_t) * in_pixel_count);
    MPI_Recv(input_data, in_pixel_count, MPI_INT16_T, 0, input_tag, MPI_COMM_WORLD, &status);

    for (int row = 1; row < rows_per_proc+1 ; row++)
    {
        for (int col = 1; col < cols - 1; col++)
        {
            filtro_pixel(input_data, rows_per_proc+2, cols, row, col, output_data);
        }
    }
    //printf("diag %d %d %d\n",vals_per_row, rows_per_proc, out_pixel_count);
    MPI_Send(output_data + vals_per_row, out_pixel_count, MPI_INT16_T, 0, output_tag, MPI_COMM_WORLD);
    free(input_data);
    free(output_data);
}

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0)
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

        filtro_master(src_image, rows, cols, out_image);

        fprintf(output, "%d\n%d\n", rows, cols);
        for (int i = 0; i < pixel_count; i++)
        {
            fprintf(output, "%d\n", out_image[i]);
        }

        free(src_image);
        free(out_image);
        fclose(input);
        fclose(output);
    }
    else
    {
        filtro_worker();
    }

    MPI_Finalize();
    return 0;
}