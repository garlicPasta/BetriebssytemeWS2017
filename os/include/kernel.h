#ifndef _KERNEL_H_
#define _KERNEL_H_

void printChar(char c);
char readChar(void);
int create_thread(thread t);
void destroy_thread(int id);
void delay_thread(int id);

#endif
