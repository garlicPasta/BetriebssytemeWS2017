#include <aic.h>
#include <system_timer.h>
#include <debug_unit.h>
#include <my_print.h>
#include <scheduler.h>
#include <threading.h>

extern volatile char *const TRANS_ADDR;

void print_count_to(int limit, char c);

static inline void print_c(char c) {
    *TRANS_ADDR = c;                        // schreibe char
}

void irq_handler() {
    acknowledge_interrupt();
    my_print_f("!");
    if (is_timer_done()){
        schedule();
    }
    aic_clear_interrupt(1);

}

void dummy_thread(int param){
    print_count_to(24, (char) param);
    finish();
}

void irq_handler_dbgu() {
        //my_print_f("Received char\n");
        char c = dbgu_getc();
        start_thread(dummy_thread, (int) c);
}

void __attribute__((interrupt("SWI"))) swi_handler(void) {
	int val=0;
	int check=7;
	asm (	"LDR	r0,[lr,#-4];"
			"BIC	%r0,r0,#0xff000000;" : "=r" ( val ));
    my_print_f("! SWI %x = %x \n",val,check);
	
	
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

