# Suma de vectores

El workload consiste en encontrar la cantidad de celdas de memoria que contienen
un 1. 

# Version sin MP

Un solo PE cuenta todas las instancias. 

REG1 addr vector1, REG2 addr vector 2

## Ubicaciones de memoria particulares 

|Col txt| Dirección | Uso                  |
|:------|:----------|----------------------|
|   1   | 0x0  | Cantidad de datos a procesar en total |
|   2   | 0x1  | Anterior valor entre cantidad de PEs            |
|   3   | 0x2  | Dir Inicio de vector 1                |
|   4   | 0x3  | Dir Inicio de vector 2                  |
|   5   | 0x4  |  0 (ID PE)                         |
|   6   | 0x5  |  1 (ID PE)                         |
|   7   | 0x6  |  2 (ID PE)                         |
|   8   | 0x7  |  3 (ID PE)                         |
|   9   | 0x8  | Scratch 0                           |
|   10  | 0x9  | Scratch 1 (puntero vres, calculado) |
|   11  | 0xa  | Scratch 2 scratch de loop var       |
|   12  | 0xb  | Scratch 3                           |
|   13  | 0xc  | Scratch 4                           |
|   14  | 0xd  | Scratch 5  (puntero vres, calc)     |
|   15  | 0xe  | Scratch 6                           |
|   16  | 0xf  | Scratch 7                          |
|   17  | 0x10 | Scratch 8     |
|   18  | 0x11 | Scratch 9     |
|   19  | 0x12 | Scratch 10     |
|   20  | 0x13 | Scratch 11     |
|   21  | 0x14 | Scratch 12     |
|   22  | 0x15 | Scratch 13     |
|   23  | 0x16 | Scratch 14     |
|   24  | 0x17 | Scratch 15     |
|   25  | 0x18 | Scratch 16     |
|   26  | 0x19 | Scratch 17     |
|   27  | 0x1a | Scratch 18     |
|   28  | 0x1b | Scratch 19     |


## Con false sharing 

in-place con cada pe sumando el mod

- Valor PE0: 8
- Valor PE1: 9
- Valor PE2: 10
- Valor PE3: 11

## Sin false sharing 

opcion1: in place
opcion2: suma no se guarda donde se lee, se guarda en donde termine el 2do vector

- Valor PE0: 8
- Valor PE1: 12
- Valor PE2: 16
- Valor PE3: 20

## Descripción general 

Cantidad de iter se carga en REG0 
Direccion de inicio se carga en REG1
Valor de carga a REG2 para chequear
REG3 mantiene el valor temporal del contador 
Dirección de inicio se reescribe con un 0 para señalar que terminó el PE de procesar