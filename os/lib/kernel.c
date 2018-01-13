#include <threading.h>
#include <debug_unit.h>

void print_char(char c){
    dbgu_putc(c);
}

char read_char(void){
    dbgu_getc();
}

int create_thread(thread t, int param){
    return start_thread(t, param);
}

int destroy_thread(int id){
    return remove_thread(id);
}

void delay_thread(int id){
}
