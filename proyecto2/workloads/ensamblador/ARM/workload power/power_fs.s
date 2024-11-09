.section .data
result: .word 1 @ Variable compartida para el resultado
exp: .word 8    @ Exponente n/N (n = potencia, N = num procesadores) 

.section .text
.global _start
_start:
    LDR r0, =exp    @ cargar la direccion de exp
    LDR r0, [r0]    @ cargar el valor de exp
    MOV r2, r0      @ inicializacion del contador de iteraciones

loop:
    LDR r1, =result     @ cargar direccion de result
    LDR r1, [r1]        @ cargar valor actual de result

    MOV r3, #2          @ equivalente a LOAD
    MUL r1, r3, r1    
    SUB r2, r2, #1      @ equivalente a DEC

    LDR r3, =result     @ cargar la direccion de result
    STR r1, [r3]        @ guardar valor calculado en result

    CMP r2, #0          @ equivalente a JNE
    BNE loop

end:
    @ syscall de exit
    MOV r7, #1        
    MOV r0, #0        
    SVC #0            
