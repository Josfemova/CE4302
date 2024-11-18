.section .text
.global _start
_start:
    MOV r0, #4  @ n potencia de 2
    MOV r1, #1  @ inicializacion del resultado
    MOV r2, r0  @ inicializacion del contador de iteraciones

loop:
    MOV r3, #2        @ equivalente a LOAD
    MUL r1, r3, r1    
    SUB r2, r2, #1    @ equivalente a DEC

    CMP r2, #0        @ equivalente a JNE
    BNE loop

end:
    @ syscall de exit
    MOV r7, #1        
    MOV r0, #0        
    SVC #0            
