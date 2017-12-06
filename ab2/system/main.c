#include <stdint.h>
#include "my_print.h"

#define INT_BYTE_COUNT 4
#define VECTOR_REG_START 0xFFFFF000
#define INTERRUPT_ENABLE 0xFFFFF120


extern volatile char *const TRANS_ADDR;
extern volatile char *const STATUS_ADDR;
extern volatile char *const RECEIV_ADDR;

volatile char *const INTR_ADDR = (char *) VECTOR_REG_START;
volatile char *const INTR_ENABLE = (char *) INTERRUPT_ENABLE;


static inline void print_c(char c) {
    *TRANS_ADDR = c;                        // schreibe char
}

static inline void handle_input(){
    for (;;) {}
}

void main(void) {
    my_print_f(">> Launched main \n");

    //asm ("swi #0");
  	//asm volatile (".word 0x07F000F0"); 
    //*(int *)0xa0000000 = 0;

    handle_input();
}
