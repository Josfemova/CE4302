#include <stdio.h>
#include <mpi.h>
#include <string.h>

// Multiplicacion de matriz 4x4
// Asume 4 nodos
#define matrix_rows 4
#define matrix_cols 4
#define matrix_sz matrix_rows *matrix_cols

void print_matrix(int *matrix)
{
    for (int i = 0; i < matrix_rows; i++)
    {
        printf("[");

        for (int j = 0; j < matrix_cols; j++)
        {
            printf(" %4d ", matrix[i * matrix_cols + j]);
        }
        printf("]\n");
    }
}

int main(int argc, char **argv)
{
    int myrank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (nprocs > matrix_sz)
    {
        return -1;
    }
    int epp = matrix_sz / nprocs;           // elementos por proceso
    int max_epp = epp + matrix_sz % nprocs; // elementos por proceso
    int start = myrank * epp;
    int end = (myrank == (nprocs - 1)) ? matrix_sz : start + epp;

    int A[matrix_sz];
    int B[matrix_sz];
    int Results[matrix_sz];

    if (myrank == 0)
    {
        for (int i = 0; i < matrix_sz; i++)
        {
            A[i] = 1 + i;
            B[i] = 5 + i;
        }
        // mientras que tenga columnas por recorre
    }
    MPI_Bcast(A, matrix_sz, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, matrix_sz, MPI_INT, 0, MPI_COMM_WORLD);
    // todos tienen los datos!

    int values[max_epp];
    int val_index = 0;

    for (int i = start; i < end; i++)
    {
        int value = 0;
        int row = i / matrix_cols;
        int col = i - matrix_cols * row;

        int *rowA = &A[row * matrix_cols];
        int *colB = &B[col];

        for (int j = 0; j < matrix_cols; j++)
        {
            value += (*(rowA + j)) * (*(colB + j * matrix_cols));
        }

        if (myrank == 0)
        {
            Results[i] = value;
        }
        else
        {
            values[val_index] = value;
            val_index++;
        }
    }

    if (myrank != 0)
    {
        MPI_Send(values, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status status;
        int value;
        for (int i = 0; i < (nprocs - 1); i++)
        {
            MPI_Recv(values, max_epp, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            int s = status.MPI_SOURCE * epp;
            int e = (status.MPI_SOURCE == (nprocs - 1)) ? matrix_sz : s + epp;
            int size = e - s;
            memcpy(Results + s, values, size * sizeof(int));
        }
        print_matrix(A);
        printf("\nX\n\n");
        print_matrix(B);
        printf("\n=\n\n");
        print_matrix(Results);
    }

    MPI_Finalize();
    return 0;
}
