#include <stdint.h>
#include "my_print.h"

#define INT_BYTE_COUNT 4
#define REMAP_REG 0xFFFFFF00
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
    for (;;) {
        if (*STATUS_ADDR & 0x01) {                           // pruefe ob es eine neue Eingabe gab
            print_c(RECEIV_ADDR[0]);
        }
    }
}

void __attribute__((interrupt("IRQ"))) irq_handler() {
    print_c('x');
}

void __attribute__((interrupt("SWI"))) swi_handler(void) {
    /*
    asm("stmdb sp, {r0-r15};"
            "sub sp,sp, #(16*4);"
            "mov r0, sp;"
    );
     */
    print_c('!');
    //asm("LDMFD sp, {r0-r15};");
}

void __attribute__((interrupt("ABORT"))) data_abort_handler(void) {
    print_c('q');
    for (;;) {}
}
void __attribute__((interrupt("ABORT"))) prefetch_abort_handler(void) {
    print_c('u');
    print_c('u');
}

void undef_handler(void) {
    print_c('q');
    print_c('q');
}

void __attribute__((interrupt("FIQ"))) fiq_handler(void) {
    print_c('q');
}

void copy_vectors(void) {
    extern uint32_t vectors_start;
    extern uint32_t vectors_end;
    uint32_t *vectors_src = &vectors_start;
    uint32_t *vectors_dst = (uint32_t *) 0x0;
    volatile uint32_t *REMAP_REGISTER = (uint32_t *) REMAP_REG;
    *REMAP_REGISTER = 1;

    //my_print_f(">> Creating IVT at 0x0\n");

    while(vectors_src < &vectors_end) {
        //my_print_f(">> Copied from: %p to %p \n",vectors_src, vectors_dst);
        *vectors_dst++ = *vectors_src++;
    }
}

void main() {
    //my_print_f(">> Lauched main \n");
    *INTR_ENABLE=1;


    asm("swi #0");
    asm(".word 0x07F00F0");

    handle_input();
}



