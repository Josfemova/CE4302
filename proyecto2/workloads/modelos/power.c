#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 4
#define N 4

// +-------------------------------------------------------+
// | Caso 1: Potencia 2 a la n sin                         |
// | multiprocessing (MP)                                  |
// +-------------------------------------------------------+

int power_sequential() {
    int result = 1;
        for (int i = 0; i < N; i++) {
        result *= 2; 
    }
    return result;
}

void case1() { 
    int result = power_sequential();
    printf("[No MP] 2^%d = %d\n", N, result);
}

// +-------------------------------------------------------+
// | Caso 2: Potencia 2 a la n con MP y false sharing      |
// +-------------------------------------------------------+

int result = 1; // Variable compartida, suceptible a false sharing

void* power_fs(void* arg) {
    for (int i = 0; i < N; i++) {
        result *= 2;
    }
    return NULL;
}

void case2() { 
    pthread_t threads[NUM_THREADS]; // Crear hilos

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, power_fs, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("[MP y false sharing] 2^%d = %d\n", N, result);
}

// +-------------------------------------------------------+
// | Caso 3: Potencia 2 a la n con MP sin false sharing     |
// +-------------------------------------------------------+

#define CACHE_LINE_SIZE 64  // Padding para evitar false sharing
int results[NUM_THREADS][CACHE_LINE_SIZE / sizeof(int)]; // Array para almacenar resultados de cada hilo

void* power(void* arg) {
    int thread_id = (int)(long)arg; // Obtener ID del hilo
    int local_result = 1;            // Inicializar result local

    // Cada hilo calcula 2^(N/NUM_THREADS)
    for (int i = 0; i < N / NUM_THREADS; i++) {
        local_result *= 2;            // Multiplica el result local por 2
    }

    results[thread_id][0] = local_result; // Guardar el resultado en el array
    return NULL;
}

void case3() {
    pthread_t threads[NUM_THREADS]; // Crear hilos

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, power, (void*)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Combinar resultados
    int final_result = 1;
    for (int i = 0; i < NUM_THREADS; i++) {
        final_result *= results[i][0]; // Multiplica los resultados de cada hilo
    }

    printf("[MP sin false sharing] 2^%d = %d\n", N, final_result);
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
    cpu_time_used = (((double)(end - start)) / CLOCKS_PER_SEC)*1000;
    printf("Tiempo de ejecucion: %f ms\n", cpu_time_used);

    return 0;
}
