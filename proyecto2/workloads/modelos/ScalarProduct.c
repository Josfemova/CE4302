#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 4
#define SIZE 1000

int a[SIZE];
int b[SIZE];

// +-------------------------------------------------------+
// | Caso 1: Producto escalar entre dos vectores sin       |
// | multiprocessing (MP), se realiza de forma secuencial  |
// +-------------------------------------------------------+

int dot_product_sequential(int* a, int* b) {
    int result = 0;
    for (int i = 0; i < SIZE; i++) {
        result += a[i] * b[i];
    }
    return result;
}

void case1 () {
    int result = dot_product_sequential(a, b);
    printf("Producto escalar: %d\n", result);
}

// +-------------------------------------------------------+
// | Caso 2: Producto escalar con MP y false sharing       |
// +-------------------------------------------------------+

int global_result_fs = 0; // Variable compartida, susceptible a false sharing

void* dot_product_fs(void* arg) {
    int thread_id = *((int*)arg); //identificador del hilo
    int segment_size = SIZE / NUM_THREADS; // tamaño del segmento para MP
    int start = thread_id * segment_size;
    int end = start + segment_size;

    for (int i = start; i < end; i++) {
        global_result_fs += a[i] * b[i]; // Acceso a variable compartida
    }
    
    return NULL;
}

void case2() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i; // Pasar ID de hilo
        pthread_create(&threads[i], NULL, dot_product_fs, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Producto escalar (con false sharing): %d\n", global_result_fs);
}

// +-------------------------------------------------------+
// | Caso 3: Producto escalar con MP sin false sharing     |
// +-------------------------------------------------------+

// Padding para evitar false sharing
int global_result[NUM_THREADS] = {0}; 

void* dot_product(void* arg) {
    int thread_id = *((int*)arg);
    int segment_size = SIZE / NUM_THREADS;
    int start = thread_id * segment_size;
    int end = start + segment_size;

    for (int i = start; i < end; i++) {
        global_result[thread_id] += a[i] * b[i]; // Resultados por hilo
    }

    return NULL;
}

void case3() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i; 
        pthread_create(&threads[i], NULL, dot_product, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int total_result = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_result += global_result[i]; // Sumar resultados de cada hilo
    }

    printf("Producto escalar (sin false sharing): %d\n", total_result);
}

// +-----------------------------------------------------------+
// | Inicialización y print para arrays (de 0 a n, n = SIZE-1) |
// +-----------------------------------------------------------+

void init_array(int* array) {
    for (int i = 0; i < SIZE; i++) {
        array[i] = i;
    }
}

void print_array(int arr[]) {
    printf("Array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {

    // Inicialización de los vectores de forma random
    init_array(a);
    init_array(b);

    // Medición del tiempo de ejecución
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Llamada a los casos de prueba
    case3();
    
    end = clock();

    // Calcular el tiempo de ejecución
    cpu_time_used = (((double)(end - start)) / CLOCKS_PER_SEC)*1000;
    printf("Tiempo de ejecucion: %f ms\n", cpu_time_used);

    return 0;
}
