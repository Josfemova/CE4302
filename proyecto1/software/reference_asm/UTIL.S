#ifndef UTIL_INCLUDE
#define UTIL_INCLUDE

#define ROM_BASE 0x0000
#define RAM_BASE 0x1000
#define STACK_START 0x1800
#define STACK_END   0x1400
#define LED_BASE 0x2000
#define SW_BASE 0x2004
#define KEY_BASE 0x2008
#define GPIO0_VAL_BASE 0x200c
#define GPIO0_DIR_BASE 0x2010
#define GPIO1_VAL_BASE 0x2014
#define GPIO1_DIR_BASE 0x2018
#define ROM_AUD1_BASE_UP21 0x20000
#define ROM_AUD2_BASE_UP21 0x40000
#define SW1_MASK 0x2
#define SW2_MASK 0x4
#define SW3_MASK 0x8


/* Funciones de utilidad */
#define NOP SUM,zero,zero,zero
#define JBA JLRL,zero,lr,0
#define MOV(rd, rs) SUM,rd,rs,zero
#define NOT(rd, rs) XORI,rd,rs,-1
#define NEG(rd, rs) DIF,rd,zero,rs
#define CALL JLL,lr
#define JMP JLL,zero
#define PUSH(rs) SUMI, sp, sp, -4; \
                 STM, rs, sp, 0; 
#define POP(rs)  LDM, rs, sp, 0; \
                 SUMI, sp, sp, 4; 

/*1ms*/
#define DELAY_T 6250 

#define GPIO0_OUTPUTS_MASK 0x5FFFF
#define READY_PIN_MASK 0x10000
#define DATA_PIN_MASK 0xFFFF
#define READY_FLAG_PIN_POS 16
#define REQUEST_PIN_POS 17
#define ACK_PIN_POS 18 
#define TRUE 1
#define FALSE 0
#define NO_OFFSET 0

#endif