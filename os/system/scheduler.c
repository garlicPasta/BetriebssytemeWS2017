#include <threading.h>
#include <scheduler.h>
#include "my_print.h"
#include "memlayout.h"

#define MAX_THREADS 2
#define SP 13
#define LR 14
#define PC 15
#define CPSR 16

int current = -1;
int next = 0;
static int count = 0;
static TCB threads[MAX_THREADS];

void schedule(){

    int* transfer_sp = (int*)REG_TRANSFER; 
    int* transfer_pc = (int*)PC_TRANSFER; 

	TCB *t;
    if (current >= 0){
        my_print_f(">> Save Register\n");
        t = (TCB*) &threads[current];
        t->sp = *transfer_sp;
        t->pc = *transfer_pc;
    }

	my_print_f(">> Find next Thread\n");

	int tried = 0;
	int found = 0;
	while (tried < MAX_THREADS){
		current = (current+1)% MAX_THREADS;	
		tried++;
		t = (TCB*) &threads[current];
		if(t->state == READY){
			found =1;
			break;
		}
	}
    if (found){
        int i = 0;
        my_print_f("Found one!\n");
        *transfer_sp = t->sp;
        *transfer_pc = t->pc;
    }else{
        my_print_f("IDLE!\n");
    }
}

int find_slot(void) {
    int i;
    for(i=0; i < MAX_THREADS; i++){
        TCB *tcb = (TCB*) &threads[i];
        if (tcb->state==DEAD){
                return i;
                }
    }
    return -1;
}

void add(thread t, int param){
	int i;

    next = find_slot();
    if (next == -1){
        my_print_f("[Exception] No empty Thread slot available\n");
        return;
    }

	TCB *tcb = (TCB*) &threads[next];
	tcb-> id = ++count;
	tcb-> state = READY;
    //
    // TODO set to remove thread routine
    int* stack_bottom = (int*) USER_STACK_BOTTOM - next * STACK_SIZE;
    *stack_bottom = param;
	tcb->sp = stack_bottom;
	tcb->pc = t;
	//tcb->registers[CPSR] = 1 << 4;
}

void finish(){
	TCB * t = &threads[current];
	t->state = DEAD;
    while(1){};
}

void init(){
    int i;
    for(i=0; i < MAX_THREADS; i++){
        TCB *tcb = (TCB*) &threads[i];
        tcb->state=DEAD;
        tcb->id=0;
    }
}

