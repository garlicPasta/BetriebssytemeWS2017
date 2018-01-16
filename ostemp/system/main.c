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
			if('a'<=c && c<='z'){
            	fibo(18);//delay_thread(20);
			}else{
				fibo(18);
			}

        };
        my_print_f("%c,",c);
    }
}

void sub_test_thread(int param){
    print_count_to(24, (char) param);
    finish();
}

void test_thread(int param){
    while(1==1){
		my_print_f("waiting for char");
		
		char c = read_char();
		my_print_f("unblocked");
		start_thread(sub_test_thread,(int) c);
		my_print_f("created");
		
	}
	finish();
}

void main(void) {
    my_print_f(">> Launched main \n");

	create_thread(test_thread,0);
    loop_forever(3);
}
