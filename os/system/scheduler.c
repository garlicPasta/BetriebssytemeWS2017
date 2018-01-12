#include <threading.h>
#include <scheduler.h>
#include "my_print.h"
#include "memlayout.h"

#define MAX_THREADS 16
#define SP 13
#define LR 14
#define PC 15
#define CPSR 16

int current = -1;
int thread_count = 0;
int isIdle = 1;
int next = 0;
static int count = 0;
static TCB threads[MAX_THREADS+1];

void _enable_interrupts(void);
void _disable_interrupts(void);

void schedule(){

    int** transfer_sp = (int**) REG_TRANSFER; 
    thread* transfer_pc = (thread*) PC_TRANSFER; 

	TCB *t;
    if (isIdle==0){
        //my_print_f(">> Save Register\n");
        t = (TCB*) &threads[current];
        t->sp = *transfer_sp;
        t->pc = *transfer_pc;
    }

	//my_print_f(">> Find next Thread\n");

	int tried = 0;
	int found = 0;
    int old = current;
	while (tried < MAX_THREADS){
		current = (current+1)% MAX_THREADS;	
		tried++;
		t = (TCB*) &threads[current];
		if(t->state == READY){
            isIdle=0;
			found = 1;
			break;
		}
	}
    if (found){
        if (current != old)
            my_print_f("\n");
    }else{
        my_print_f("IDLE!\n");
		t = (TCB*) &threads[MAX_THREADS + 1];
    }
    *transfer_sp = t->sp;
    *transfer_pc = t->pc;
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

int find_slot_of_thread_by(int id) {
    int i;
    for(i=0; i < MAX_THREADS; i++){
        TCB *tcb = (TCB*) &threads[i];
        if (tcb->id==id){
                return i;
                }
    }
    return -1;
}

int add(thread t, int param){
    next = find_slot();
    if (next == -1){
        my_print_f("[Exception] No empty Thread slot available\n");
        return -1;
    }

	TCB *tcb = (TCB*) &threads[next];
	tcb-> id = ++count;
	tcb-> state = READY;
    int* stack_bottom = (int*) USER_STACK_BOTTOM - next * STACK_SIZE;
    *stack_bottom = param;
    *(stack_bottom-4) = param;
	tcb->sp = stack_bottom;
	tcb->pc = t;
    //my_print_f(">> Adding thread with param %c in stack %x \n", (char) param, stack_bottom);
    thread_count++;
    return tcb-> id;
}

int remove(int id){
    int slot_index = find_slot_of_thread_by(id);
    if (slot_index < 0) 
        return -1;

    TCB* tcb = &threads[slot_index];
    tcb->state = DEAD;
    thread_count--;
    if (thread_count==0){
        isIdle = 1;
    }
    return 0;
}

void finish(){
	TCB * t = &threads[current];
    //my_print_f("CLosed thread %x", current);
	t->state = DEAD;
    thread_count--;
    if (thread_count==0){
        isIdle = 1;
    }
    for(;;){};
}

static void idle(){
    for (;;) {}
}

void init_scheduler(){
    int i;
    for(i=0; i < MAX_THREADS; i++){
        TCB *tcb = (TCB*) &threads[i];
        tcb->state=DEAD;
        tcb->id=0;
    }
    TCB *tcb = (TCB*) &threads[MAX_THREADS+1];
	tcb->sp = (int*) USER_STACK_BOTTOM - (MAX_THREADS+1) * STACK_SIZE;;
	tcb->pc = idle;
}

