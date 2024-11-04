## Prerequisitos

- `gcc`
- `docker`
- `docker-compose`
-  bibliteca `opencv-python` disponible en ambiente para importar `cv2` en scripts
-  bibliteca de python `numpy` 
- `mpich` o `openMPI` instalados de forma que `mpicc` y `mpiexec` estén disponibles en sistema.
- ambiente linux con el comando `xdg-open` disponible. 

## Configuración inicial

Solo es necesario crear un registry de docker como lo indica el repo `docker-mpi`
empotrado en el entregable (paso se hace solo una vez):

```Shell 
docker service create --name registry --publish published=5000,target=5000 registry:2
```

El resto de configuraciones son automatizadas con scripts y makefiles.

## Multiplicación Matrices 4x4

Los archivos se encuentran dentro de esta carpeta, en 
`./matrix4x4`.

El algoritmo se implementa en el archivo `matrix_mul.c`

Se provee un Makefile que se encarga de todo el proceso de compilación/ejecucion. 
Los resultados se muestran en la misma consola. Capturas de los resultados se 
adjuntan en el informe. 

Ejecute

```Shell
make test
```

Para ver todos los subcomandos disponibles. La prueba completa se puede ejecutar con

```Shell
make run_docker
```

## Filtro de sobel

Los archivos para esto se encuentran en `./sobel_filter`. Se proveen las implementaciones:

- Serial: `serial.c`
- Paralela local con openmp: `parallel_omp.c`
- Paralela local o remota con MPI: `parallel_mpi.c`

Todas usan el filtro horizontal y vertical.

Estos programas asumen como primer argumento de consola un archivo de texto de entrada, generado con el script `img2file.py`, y de segundo argumento de consola el archivo de salida el cual luego se pasa por `file2img.py` para generar un .png final que muestre el resultado de ejecución del algoritmo. 

### Ejecución

Para ejecutar un test solo se requiere ejecutar 

```Bash
make test TEST=<# de test, puede ser 1,2,3,4,5>
```

Esto generará los archivos de entrada para el algoritmo, lo ejecutará, medirá
su tiempo de ejecución y obtendrá la imagen final generada. Las imágenes de 
entrada se componen en su mayoría por recursos obtenidos de 
<https://r0k.us/graphics/kodak/>.
