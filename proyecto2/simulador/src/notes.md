# Formato de instrucciones

a. LOAD REG<n> addr # carga datos memoria[addr] en registro n
b. STORE REG<n> addr # escribe en memoria registro n
c. INC REG<n> # incremento registro n
d. DEC REG<n> # decremento registro n
e. JNZ [label] # salto condicional
f. label: # Asi se usa el label

## Instrucciones dentro del CPU

a. LOAD <n>
b. STORE <n>
c. INC <n>
d. DEC <n>
e. JNZ <pc donde se ubica el label>
f. label:

# Uso de JNZ

Jump Not Zero utiliza el Registro 1 como condición. Es decir si el registro 1 es diferente de 0 va a saltar, si es igual a 0 no se realiza el salto. En el archivo de tests.cpp se puede ver varios ejemplos del uso del JNZ, tambien de un loop creado con esta instrucción.
