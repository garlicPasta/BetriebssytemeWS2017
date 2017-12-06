#define REMAP_REG 0xFFFFFF00
#define ST_BASE 0xFFFFFD00

#include <stdint.h>
#include <my_print.h>
#include "memlayout.h"

enum psr_mode {
 	/* Modusbits aufgeschlüsselt */ 
 	PSR_USR = 0x10,
 	PSR_FIQ = 0x11,
 	PSR_IRQ = 0x12,
 	PSR_SVC = 0x13,
 	PSR_ABT = 0x17,
 	PSR_UND = 0x1b,
 	PSR_SYS = 0x1f,
};

void _set_stack(enum psr_mode mode, unsigned int addr);
void _enable_interrupts(void);

void copy_vectors(void) {
    extern uint32_t vectors_start;
    extern uint32_t vectors_end;
    uint32_t *vectors_src = &vectors_start;
    uint32_t *vectors_dst = (uint32_t *) 0x0;
    volatile uint32_t *REMAP_REGISTER = (uint32_t *) REMAP_REG;
    *REMAP_REGISTER = 1;

    my_print_f(">> Creating IVT at 0x0... ");

    while(vectors_src < &vectors_end) {
        //my_print_f(">> Copied from: %p to %p \n",vectors_src, vectors_dst);
        *vectors_dst++ = *vectors_src++;
    }
    my_print_f("done\n");
}

void init_stacks(void){
    my_print_f(">> Init stacks... ");
 	_set_stack(PSR_FIQ, STACK_BOTTOM - 1 * STACK_SIZE); 
 	_set_stack(PSR_IRQ, STACK_BOTTOM - 2 * STACK_SIZE); 
 	_set_stack(PSR_ABT, STACK_BOTTOM - 3 * STACK_SIZE); 
 	_set_stack(PSR_UND, STACK_BOTTOM - 4 * STACK_SIZE); 
 	_set_stack(PSR_SYS, STACK_BOTTOM - 5 * STACK_SIZE); 
    my_print_f("done\n");
}

void init_cpu(void){
    copy_vectors();
    init_stacks();
    _enable_interrupts();
    //init_time_interrupt();
}
