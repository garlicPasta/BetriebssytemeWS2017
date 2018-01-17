#include <threading.h>
#include <debug_unit.h>


void print_char(char c){
    dbgu_putc(c);
}

char read_char(int* buffer){
    asm("swi #10");
    register char c asm("r0");
	char t = (char) *buffer;
    return t;
}

int create_thread(thread t, int param){
    return start_thread(t, param);
}

int destroy_thread(int id){
    return kill_thread(id);
}

void delay_thread(int id){
}
