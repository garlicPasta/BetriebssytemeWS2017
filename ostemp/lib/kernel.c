#include <kernel.h>
#include <threading.h>
#include <scheduler.h>
#include <debug_unit.h>
#include <my_print.h>

#define PRINT_CHAR 10
#define READ_CHAR 11
#define CREATE_THREAD 12
#define DESTROY_THREAD 13
#define DELAY_THREAD 14

void print_char(char c){
	asm("swi #10");
    dbgu_putc(c);
}

char read_char(void){
	block_current_on_char();
	//asm("swi #11");
    char c = dbgu_getc();
	return c;
}

int create_thread(thread t, int param){
	//asm("swi #12");
	my_print_f("create Thread");
    int id = start_thread(t, param);
	return id;
}

int destroy_thread(int id){
	//asm("swi #13");
    return kill_thread(id);
}

void delay_thread(int id){
	//asm("swi #14");
	block_current_on_timer(id);
	//asm("swi #14");
	schedule();
}


void handle_print_char(){}
void handle_read_char(){
	
}
void handle_create_thread(){}
void handle_destroy_thread(){}
void handle_delay_thread(){}

void handle_swi(int code){
	my_print_f("! SWI(%x) \n",code);
	switch(code){
		case PRINT_CHAR:
			handle_print_char();
			break;
		case READ_CHAR:
			handle_read_char();
			break;
		case CREATE_THREAD:
			handle_create_thread();
			break;
		case DESTROY_THREAD:
			handle_destroy_thread();
			break;
		case DELAY_THREAD:
			handle_delay_thread();
			break;
		default:
			my_print_f("UNKNOWN SWI");
	}

}























