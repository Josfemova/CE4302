#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define SIZE 1000000 // Size del array
int array[SIZE];  // Array de entrada

// +-------------------------------------------------------+
// | Caso 1: Suma de los elementos de un arrya sin         |
// | multiprocessing (MP), se realiza de forma secuencial  |
// +-------------------------------------------------------+

int sum_sequential() {
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        total += array[i];  
    }
    return total;
}

void case1() {
    printf("Suma total secuencial: %d\n", sum_sequential());
}

// +-------------------------------------------------------+
// | Caso 2: Suma paralela con MP y false sharing          |
// +-------------------------------------------------------+

int partial_sum_fs[NUM_THREADS];  // Suma parcial para cada hilo (contiguo en memoria, suceptible a fs)

void *parallel_sum_fs(void *arg) {
    int id = *(int *)arg;
    int start = id * (SIZE / NUM_THREADS);
    int end = start + (SIZE / NUM_THREADS);

    for (int i = start; i < end; i++) {
        partial_sum_fs[id] += array[i];  // Acumulamos en variables contiguas
    }
    return NULL;
}

void case2() {
    pthread_t threads[NUM_THREADS];
    int threads_ids[NUM_THREADS];

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        threads_ids[i] = i;
        pthread_create(&threads[i], NULL, parallel_sum_fs, &threads_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sumar los resultados parciales
    int total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total += partial_sum_fs[i];  // False sharing puede ocurrir aquí
    }

    printf("Suma total (con false sharing): %d\n", total);
}

// +-------------------------------------------------------+
// | Caso 3: Producto escalar con MP sin false sharing     |
// +-------------------------------------------------------+

#define CACHE_LINE_SIZE 64 // Separa cada cada suma parcial por al menos 64 bytes (padding)
int partial_sum[NUM_THREADS][CACHE_LINE_SIZE / sizeof(int)]; 

void *parallel_sum(void *arg) {
    int id = *(int *)arg;
    int start = id * (SIZE / NUM_THREADS);
    int end = start + (SIZE / NUM_THREADS);

    for (int i = start; i < end; i++) {
        partial_sum[id][0] += array[i];  // Alineado para evitar compartir caché
    }
    return NULL;
}

int case3() {
    pthread_t threads[NUM_THREADS];
    int threads_ids[NUM_THREADS];

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        threads_ids[i] = i;
        pthread_create(&threads[i], NULL, parallel_sum, &threads_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sumar los resultados parciales
    int total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total += partial_sum[i][0];  // No hay false sharing
    }

    printf("Suma total (Sin false sharing): %d\n", total);  // Debería ser 1000
    return 0;
}


int main() {

    // Inicializar el array con valores
    for (int i = 0; i < SIZE; i++) {
        array[i] = 1;  // Ejemplo: inicializamos el array con 1s
    }

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
