#ifndef _KERNEL_H_
#define _KERNEL_H_
#include <threading.h>
void print_char(char c);
char read_char(void);
int create_thread(thread t,int param);
int destroy_thread(int id);
void delay_thread(int id);
void handle_swi(int code);

#endif
