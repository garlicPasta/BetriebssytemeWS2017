#include <stdint.h>
#include <math.h>
#include <debug_unit.h>
#include <threading.h>
#include <scheduler.h>
#include "my_print.h"

#define INT_BYTE_COUNT 4
#define VECTOR_REG_START 0xFFFFF000
#define INTERRUPT_ENABLE 0xFFFFF120
#define DELAY 2000 //4294967295


extern volatile char *const TRANS_ADDR;
extern volatile char *const STATUS_ADDR;
extern volatile char *const RECEIV_ADDR;

volatile char *const INTR_ADDR = (char *) VECTOR_REG_START;
volatile char *const INTR_ENABLE = (char *) INTERRUPT_ENABLE;

void print_loop_for(char c);


static inline void loop_forever(){
    char c;
    for (;;) {
        c = dbgu_getc();
        print_loop_for(c);
    }
}

void print_loop_for(char c) {
    unsigned int i,j;

    for (i = 0; i< 32; i++){
        for (j=0; j<DELAY; j++) {
            fibo(18);
        };
        dbgu_putc(c);
    }
}

void print_count_to(int limit, char* t_name) {
    unsigned int i,j;

    for (i = 0; i< limit; i++){
        for (j=0; j<DELAY; j++) {
            fibo(18);
        };
        my_print_f("%s:%x ,",t_name,i);
    }
}

void dummy_thread(int param){
    asm volatile ("mov r9, #0x18");
    my_print_f("Thread 1: ");
    my_print_f("%c", (char) param);
    print_count_to(16, "Thread 1");
    finish();
}

void dummy_thread_2(int param){
    my_print_f("Thread 2: ");
    my_print_f("%c", (char) param);
    print_count_to(18, "Thread 2");
    finish();
}

void main(void) {
    my_print_f(">> Launched main \n");

    start_thread(dummy_thread, 'o');
    start_thread(dummy_thread_2, 'p');

    //asm ("swi #0");
  	asm volatile ("mov r8,#0x18");
    //*(int *)0xa0000000 = 0;

    loop_forever();
}
