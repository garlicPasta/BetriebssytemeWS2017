
#include <my_print.h>

extern volatile char *const TRANS_ADDR;

static inline void print_c(char c) {
    *TRANS_ADDR = c;                        // schreibe char
}

void __attribute__((interrupt("IRQ"))) irq_handler() {
    my_print_f("! IRQ\n");
}

void __attribute__((interrupt("SWI"))) swi_handler(void) {
    my_print_f("! SWI\n");
}

void __attribute__((interrupt("ABORT"))) data_abort_handler(void) {
    my_print_f("! DATA ABORT\n");
    for (;;) {}
}
void __attribute__((interrupt("ABORT"))) prefetch_abort_handler(void) {
    my_print_f("! PREFETCH ABORT\n");
}

void __attribute__((interrupt("UNDEF"))) undef_handler(void) {
    my_print_f("! UNDEFINED INSTRUCTION\n");
}

void __attribute__((interrupt("FIQ"))) fiq_handler(void) {
    my_print_f("! FAST INTERRUPT\n");
}

void handle_timer(void){
    print_c('t');
}

