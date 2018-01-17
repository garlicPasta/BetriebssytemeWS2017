#ifndef _KERNEL_H_
#define _KERNEL_H_

#define READ_CHAR 10
#define PRINT_CHAR 11
#define CREATE_THREAD 12
#define KILL_THREAD 13
#define DELAY_THREAD 14


void print_char(char c);
char read_char(int* buffer);
int create_thread(thread t);
void destroy_thread(int id);
void delay_thread(int id);

#endif
