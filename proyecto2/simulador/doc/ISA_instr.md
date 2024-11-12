# Formato de instrucciones

a. LOAD REG<n> addr # carga datos memoria[addr] en registro n
b. STORE REG<n> addr # escribe en memoria registro n
c. INC REG<n> # incremento registro n
d. DEC REG<n> # decremento registro n
e. JNZ [label] # salto condicional
f. label: # Asi se usa el label
g. MUL REG<n1> REG<n2>:: # Multiplicación de registros

## Instrucciones dentro del CPU (dentro de la lógica del CPU no hace falta para los workloads)

a. LOAD <n>
b. STORE <n>
c. INC <n>
d. DEC <n>
e. JNZ <pc>

# Registros

Todos los registros están inicializados en 0.

# Uso de JNZ

Jump Not Zero utiliza el registro utilizado en la ultima operación de DEC o INC. Es decir si el registro es diferente de 0 va a saltar, si es igual a 0 no se realiza el salto. En el archivo de tests.cpp se puede ver varios ejemplos del uso del JNZ, tambien de un loop creado con esta instrucción.
