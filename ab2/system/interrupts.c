#define ST_BASE 0xFFFFFD00
#define AIC_BASE 0xFFFFF000

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
    print_c('p');
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

struct ST{
 	unsigned int CR;
 	unsigned int PIMR;
 	unsigned int UNUSED[3];
 	unsigned int INTERRUPTED_ENABLE;
};

static volatile struct ST * const st = (struct ST *)ST_BASE; 

void init_time_interrupt(void) {
    st->PIMR = 10;
    st->INTERRUPTED_ENABLE = 1 << 0;
}


struct AIC{
 	unsigned int SMR[32];
 	void (*SVR[32])(void);
 	unsigned int IVR;
 	unsigned int UNUSED[8];
 	unsigned int IECR;
 	unsigned int IDCR;
 	unsigned int ICCR;
 	unsigned int ISCR;
};

static volatile struct AIC * const aic = (struct AIC *)AIC_BASE;


void init_aic(void){
    aic->IDCR = 1 << 1;
    aic->IECR = 7;

    aic->SVR[1] = handle_timer;
    aic->SMR[1] = 3;

    aic->ICCR = 1 << 1;
}