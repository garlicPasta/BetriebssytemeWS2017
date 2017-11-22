#define DEBUG_REG 0xFFFFF200
#define REMAP_REG 0xFFFFFF00 
#define WRITE_OFF 28
#define STATUS_OFF 20
#define RECEIVE_OFF 24
#define INT_BYTE_COUNT 4
#define VECTOR_REG_START 0xFFFFF000

#define IVT_RESET 	0x00000000 // RESET
#define IVT_UNDEFINED   0x00000004 // UNDEFINED INSTRUCTION
#define IVT_SOFTWARE	0x00000008 // SOFTWARE INTERRUPT
#define IVT_PREFETCH 	0x0000000C // PREFETCH ABORT
#define IVT_DATA	0x00000010 // DATA ABORT
#define IVT_IRQ		0x00000018 // IRQ
#define IVT_FIQ 	0x0000001C // FIQ evtl direkt behandlen danach ist

#include <stdarg.h>


volatile char *const TRANS_ADDR = (char *) DEBUG_REG + WRITE_OFF;
volatile char *const STATUS_ADDR = (char *) DEBUG_REG + STATUS_OFF;
volatile char *const RECEIV_ADDR = (char *) DEBUG_REG + RECEIVE_OFF;

volatile char *const INTR_ADDR = (char *) VECTOR_REG_START;

static inline void print_c(char c) {
    *TRANS_ADDR = c;                        // schreibe char
}

void irq_handler(){
    print_c('x');
}

void c_entry() {
    int i;
    for (i=0; i<32; i++){
        *(INTR_ADDR+i*4) = irq_handler;
    }

    for (;;) {
        if (*STATUS_ADDR & 0x01) {                           // pruefe ob es eine neue Eingabe gab
            print_c(RECEIV_ADDR[0]);
        }
    }
}

