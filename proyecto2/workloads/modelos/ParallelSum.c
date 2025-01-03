#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define SIZE 1000000 // Número de elementos para contar

// +-------------------------------------------------------+
// | Caso 1: Contar 1s sin multiprocessing (MP)            |
// | Se realiza de forma secuencial                         |
// +-------------------------------------------------------+
int count_ones_sequential() {
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        total++;  
    }
    return total;
}

void case1() {
    printf("[Sin MP] Total: %d\n", count_ones_sequential());
}

// +-------------------------------------------------------+
// | Caso 2: Contar 1s en paralelo con MP y false sharing   | 
// +-------------------------------------------------------+

// Variable compartida para contar unos, susceptible a false sharing
int shared_count = 0;

void *count_ones_fs(void *arg) {
    int id = *(int *)arg;
    int start = id * (SIZE / NUM_THREADS);
    int end = start + (SIZE / NUM_THREADS);

    for (int i = start; i < end; i++) {
        shared_count++;  // Incrementar la variable compartida
    }
    return NULL;
}

void case2() {
    pthread_t threads[NUM_THREADS];
    int threads_ids[NUM_THREADS];

    // Inicializar la variable compartida
    shared_count = 0; 

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        threads_ids[i] = i;
        pthread_create(&threads[i], NULL, count_ones_fs, &threads_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("[MP con false sharing] Total: %d\n", shared_count);
}

// +-------------------------------------------------------+
// | Caso 3: Contar 1s en paralelo con MP sin false sharing | 
// +-------------------------------------------------------+

#define CACHE_LINE_SIZE 64 // Separa cada contador parcial por al menos 64 bytes (padding)
int partial_count[NUM_THREADS][CACHE_LINE_SIZE / sizeof(int)]; 

void *count_ones(void *arg) {
    int id = *(int *)arg;
    int start = id * (SIZE / NUM_THREADS);
    int end = start + (SIZE / NUM_THREADS);

    for (int i = start; i < end; i++) {
        partial_count[id][0]++;  // Contar sin false sharing
    }
    return NULL;
}

void case3() {
    pthread_t threads[NUM_THREADS];
    int threads_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        partial_count[i][0] = 0; // Inicializar cada contador parcial
    }

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        threads_ids[i] = i;
        pthread_create(&threads[i], NULL, count_ones, &threads_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sumar los resultados parciales
    int total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total += partial_count[i][0]; 
    }

    printf("[MP sin false sharing] Total: %d\n", total);
}

int main() {
    // Medición del tiempo de ejecución
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Llamada a los casos de prueba
    case3(); 
    
    end = clock();

    // Calcular el tiempo de ejecución
    cpu_time_used = (((double)(end - start)) / CLOCKS_PER_SEC) * 1000;
    printf("Tiempo de ejecucion total: %f ms\n", cpu_time_used);

    return 0;
}
