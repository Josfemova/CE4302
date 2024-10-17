#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <math.h>

//Cuda error checking - non mandatory
void cudaCheckError() {
 cudaError_t e=cudaGetLastError();
 if(e!=cudaSuccess) {
   printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));
   exit(0); 
 }
}


int16_t op_x[9]={
    -1, 0, 1,
    -2, 0, 2,
    -1 ,0, 1  
}; 

int16_t op_y[9]={
    -1,-2,-1,
     0, 0, 0,
     1, 2, 1  
}; 

__global__
void filtro_pixel(int16_t* matriz_entrada, int16_t* opx, int16_t* opy, int16_t* matriz_salida){
    int row = blockIdx.x;
    int col = threadIdx.x;
    int cols = blockDim.x;
    int rows = gridDim.x;

    // curr = row*col - 1
    // prev = (row-1)*col - 1
    // post = (row+1)*col - 1
    int curr = row*cols + col-1;
    if(col == 0 || col == (cols-1) || row ==0 || row ==(rows-1)){
        // no procesar bordes
        *(matriz_salida + curr+1) = *(matriz_entrada + curr+1);
        return;
    }
    int16_t* prow0 = matriz_entrada + curr - cols;
    int16_t* prow1 = matriz_entrada + curr;
    int16_t* prow2 = matriz_entrada + curr + cols;
    int16_t result;
    int resultx = 0;
    int resulty = 0; 
    //opx y opy tienen que se copiadas a mem de la gpu duuuhh

    for (int i=0; i<3; i++){
        resultx += *(prow0+i) * opx[0+i];
        resultx += *(prow1+i) * opx[3+i];
        resultx += *(prow2+i) * opx[6+i];
        resulty += *(prow0+i) * opy[0+i];
        resulty += *(prow1+i) * opy[3+i];
        resulty += *(prow2+i) * opy[6+i];
    }
    result = (int16_t)(sqrt((double)(resultx*resultx + resulty*resulty)));
    if(result > 255) result = 255;
    matriz_salida[curr+1] = (result);
}

// Recibe la imagen con padding, y la guarda con padding 
void filtro(int16_t* matriz_entrada, int rows, int cols, int16_t* matriz_salida){

    int16_t* mat_entrada_gpu;
    int16_t* mat_salida_gpu;
    int16_t* opx;
    int16_t* opy;
    int pixel_count=rows*cols;

    int mem_size = pixel_count * sizeof(int16_t);

    // reservar memoria en gpu
    cudaMalloc((void **) &mat_entrada_gpu, mem_size);
    cudaMalloc((void **) &mat_salida_gpu, mem_size);
    cudaMalloc((void **) &opx, 9*sizeof(int16_t));
    cudaMalloc((void **) &opy, 9*sizeof(int16_t));

    // copiar datos de entrada para kernel a memoria de gpu
    cudaMemcpy(mat_entrada_gpu, matriz_entrada, mem_size, cudaMemcpyHostToDevice);
    cudaMemcpy(opx,op_x, sizeof(op_x), cudaMemcpyHostToDevice);
    cudaMemcpy(opy,op_y, sizeof(op_y), cudaMemcpyHostToDevice);
    
    dim3 dimBlock(rows,1,1);
    dim3 dimGrid(cols,1,1);

    // ejercutar kernel
    filtro_pixel<<<rows,cols>>>(mat_entrada_gpu, opx, opy, mat_salida_gpu);
    cudaDeviceSynchronize();
    cudaCheckError();

    // copiar resultados a la matriz de salida
    cudaMemcpy(matriz_salida, mat_salida_gpu, mem_size, cudaMemcpyDeviceToHost);

    // liberar recursos 
    cudaFree(mat_entrada_gpu);
    cudaFree(mat_salida_gpu);
    cudaFree(opx);
    cudaFree(opy);

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
    int pixel_count = rows*cols; 
    
    int16_t* src_image = (int16_t*)calloc((pixel_count), sizeof(int16_t));
    int16_t* out_image = (int16_t*)calloc(pixel_count, sizeof(int16_t));

    if(src_image == NULL || out_image == NULL) return 3;
    int i = 0;
    int pixel;
    
    while(fscanf(input, "%d\n", &pixel)!=EOF){
        src_image[i] = pixel;
        ++i;
    }
 
    // Los movimientos de memoria son parte del tradeoff. La carga del archivo a memoria de CPU no
    // cuenta, pero para ser justos, el movimiento de datos a memoria de GPU si se va a contar
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
