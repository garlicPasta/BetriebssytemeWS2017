#include <stdint.h>
#include <math.h>
#include <debug_unit.h>
#include <threading.h>
#include <scheduler.h>
#include <kernel.h>
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


static void loop_forever(){
    for (;;) {}
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

void print_count_to(int limit, char c) {
    int i,j;

    for (i = 0; i< limit; i++){
        for (j=0; j<DELAY; j++) {
            fibo(18);
        };
        my_print_f("%c,",c);
    }
}

void main(void) {
    my_print_f(">> Launched main \n");
    loop_forever(3);
}
