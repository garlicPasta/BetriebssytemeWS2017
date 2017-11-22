#include <stdint.h>
#include "my_print.h"

#define REMAP_REG 0xFFFFFF00 
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

void __attribute__((interrupt)) irq_handler() {
    print_c('x');
}

/* all other handlers are infinite loops */
void __attribute__((interrupt)) undef_handler(void) { for(;;); }
void __attribute__((interrupt)) swi_handler(void) { for(;;); }
void __attribute__((interrupt)) prefetch_abort_handler(void) { for(;;); }
void __attribute__((interrupt)) data_abort_handler(void) { for(;;); }
void __attribute__((interrupt)) fiq_handler(void) { for(;;); }

void copy_vectors(void) {
    extern uint32_t vectors_start;
    extern uint32_t vectors_end;
    uint32_t *vectors_src = &vectors_start;
    uint32_t *vectors_dst = (uint32_t *) 0;
    my_print_f("c");
    my_print_f("Source Pointer: %p",&vectors_start);
    my_print_f("DestPointer Pointer: %p",&vectors_end);

    while(vectors_src < &vectors_end)
        *vectors_dst++ = *vectors_src++;
    print_c('p');
}

void main() {
    *INTR_ENABLE=0;

    for (;;) {
        if (*STATUS_ADDR & 0x01) {                           // pruefe ob es eine neue Eingabe gab
            print_c(RECEIV_ADDR[0]);
        }
    }
}

