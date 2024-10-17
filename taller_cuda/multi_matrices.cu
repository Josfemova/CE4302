#include <stdio.h>
#include <cuda_runtime.h>

#define N 4

__global__ void matrixMul(int *a, int *b, int *c) { // Crea el kernel
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    int sum = 0;
    if(row < N && col < N) {
        for (int i = 0; i < N; i++) {
            sum += a[row * N + i] * b[i * N + col];
        }
        c[row * N + col] = sum;
    }
}

int main() {
    int a[N*N], b[N*N], c[N*N];      // Matrices en el host (CPU)
    int *d_a, *d_b, *d_c;            // Matrices en el device (GPU)
    int size = N * N * sizeof(int);

    for (int i = 0; i < N * N; i++) {
        a[i] = i + 3;  // cambiar los valores para probar
        b[i] = i + 6;  
    }

    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(N, N);  // 4x4 hilos por bloque
    dim3 blocksPerGrid(1, 1);    // 1x1 bloques por grid

    matrixMul<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c);

    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    printf("Resultado de la multiplicación:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", c[i * N + j]);
        }
        printf("\n");
    }
    
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}
