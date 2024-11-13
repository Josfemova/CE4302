.section .text
.global _start
_start:
    MOV r0, #1000   @ cantidad de 1s
    MOV r1, #0      @ inicializar el resultado
    MOV r2, r0      @ inicializar el contador

loop:
    ADD r1, r1, #1  @ INC
    SUB r2, r2, #1  @ DEC

    CMP r2, #0        @ equivalente a JNE
    BNE loop

end:
    @ syscall de exit
    MOV r7, #1        
    MOV r0, #0        
    SVC #0   
