# Taller Cuda 

## Compilación

Los pasos para compilar y ejecutar un test están automatizados con Make. Como
prerequisito para correr cualquier prueba, se necesita crear la carpeta /build
en el directorio del taller, esto se puede hacer manualmente o ejecutando 
`make clean`.

Para el proceso de compilación se asume el ambiente de desarrollo es la Jetson
Nano provista. Esto tiene las consecuencias de que como requisitos se tiene que

- Se asume ambiente Linux
- El cpu del ambiente soporta instrucciones ARM Neon 
- GCC está instalado y tiene soporte para intrinsics de Neon 
- Los drivers de NVIDIA están instalados, y el paquete de soporte de CUDA que 
  permite tener nvcc en `PATH` está instalado. NVCC debe tener una versión que 
  soporte código hecho para NVCC 10.2

Tanto el código serial como el que usa Neon requieren de ser linkeados con 
libmath, el Makefile se encarga de asegurarse de que dicho requisito se cumpla

## Ejecución

Para ejecutar un test solo se requiere ejecutar 

```Bash
make test TEST=<# de test, puede ser 1,2,3,4,5>
```

Esto generará los archivos de entrada para el algoritmo, lo ejecutará, medirá
su tiempo de ejecución y obtendrá la imagen final generada. Las imágenes de 
entrada se componen en su mayoría por recursos obtenidos de 
<https://r0k.us/graphics/kodak/>. 
## Benchmarking

Cada test usa `<time.h>` para estimar su tiempo de ejecución, y lo imprime en 
pantalla.

El tiempo de ejecución considera solo costos del acercamiento utilizado - no se 
toma en cuenta la carga de datos a la matriz de entrada, pero si el 
acercamiento requiere cargas a memorias particulares como en el caso de CUDA, 
como esto es parte del "precio" de usar dicho acercamiento, se valora dentro de 
la medición.


## Caso especial: multiplicación de matrices usando CUDA 

En este caso, no se automatiza la compilación ejecución, la guía general es, en 
una máquina con el ambiente descrito al inicio, para compilar y ejecutar:

```Bash
nvcc multi_matrices.cu -o build/multi_matrices
./build/multi_matrices
```

La ejecución también puede ser realizada desde la herramienta en línea 
Compiler explorer: <https://godbolt.org/z/bdo4b1dPb> 

