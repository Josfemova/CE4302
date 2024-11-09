.section .data
result: .word 0     @ Variable compartida para el resultado
ones: .word 1000    @ Cantidad de 1s que contar / N procesadores 

.section .text
.global _start
_start:
    LDR r0, =ones   @ cargar direccion de ones
    LDR r0, [r0]    @ cargar valor de ones
    MOV r1, #0      @ inicializar el resultado
    MOV r2, r0      @ inicializar el contador

loop:
    ADD r1, r1, #1  @ INC
    SUB r2, r2, #1  @ DEC

    CMP r2, #0        @ equivalente a JNE
    BNE loop

end:
    @ Faltaria concatenar los resultados

    LDR r3, =result     @ cargar la direccion de result
    STR r1, [r3]        @ guardar el resultado (r1) en la direccion de result

    @ syscall de exit
    MOV r7, #1        
    MOV r0, #0        
    SVC #0   

