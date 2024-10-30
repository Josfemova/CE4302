.section .text
.global _start
_start:
    MOV r0, #4
    MOV r1, #1
    MOV r2, r0

loop:
    MOV r3, #2
    MUL r1, r3, r1
    SUB r2, r2, #1

    CMP r2, #0
    BNE loop

end:
    LDR r3, =0x100
    STR r1, [r3]
