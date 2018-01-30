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
    for (;;) {/*my_print_f("M");fibo(18);*/}
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
int nop(int i){
	return i+1;	
}

void print_count_to_passive(int limit, int c) {
    int i;
	my_print_f("",&limit, &c);
	for (i = 0; i< limit; i++){
        
        delay_thread(250);

        my_print_f("%c,",(char)c);
    }
}

void sub_active_thread(int param){
		
    char c = (char)param;
    print_count_to(7, c);
    finish();
}

void sub_passive_thread(int param){
		
    //char c = (char)param;
    print_count_to_passive(7,param);
    finish();
}

void test_thread(int param){
	int b = 0;
	char c;
	//my_print_f("b %x",&b);
	while(1){
		c = read_char(&b);
		
		if('A' <= c && c<= 'Z'){
			start_thread(sub_active_thread,0+ (int) c);
		}else{
			start_thread(sub_passive_thread,0+ (int) c);

		}
		
	}
    finish();
}

void main(void) {
    my_print_f(">> Launched main %i %i\n",137,0);
	start_thread(test_thread, 0);
    loop_forever(3);
}
