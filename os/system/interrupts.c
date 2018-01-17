#include <aic.h>
#include <system_timer.h>
#include <debug_unit.h>
#include <my_print.h>
#include <scheduler.h>
#include <threading.h>
#include <kernel.h>

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
	int b = 0;
	//my_print_f("b %x",&b);
    char c = read_char(&b);
    print_count_to(7, c);
    finish();
}

void irq_handler_dbgu() {
        //my_print_f("Received char\n");
        char c = dbgu_getc();
		if(c=='p'){
			start_thread(dummy_thread, (int) c);
		}else{
			process_unblocking(c);
					
		}
        
}

void swi_handler(int param, int* buffer) {
	//my_print_f("bswi %x",buffer);
    switch(param) {
        case READ_CHAR:
            my_print_f("[SysCall] Read_Char");
			process_blocking(buffer);
            schedule();
            break;
        case PRINT_CHAR:
            my_print_f("[SysCall] Print_Char");
            break;
        case CREATE_THREAD:
            break;
        case KILL_THREAD:
            break;
        case DELAY_THREAD:
            break;
    }
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

