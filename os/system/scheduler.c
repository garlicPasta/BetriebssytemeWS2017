#include <threading.h>
#include <scheduler.h>
#include <system_timer.h>
#include "my_print.h"
#include "memlayout.h"

#define MAX_THREADS 16
#define IDLE_SLOT 16
#define SP 13
#define LR 14
#define PC 15
#define CPSR 16

int current = 0;
int thread_count = 0;
int isIdle = 1;
static int count = 0;
static int blocked = 0;

static int dc = (int)'?';
static TCB threads[MAX_THREADS+1];

void _enable_interrupts(void);
void _disable_interrupts(void);


static void idle(){
    for (;;) {/*my_print_f("I");fibo(18);*/}
}

void process_blocking(int* buffer){
	int** transfer_sp = (int**) REG_TRANSFER; 
    thread* transfer_pc = (thread*) PC_TRANSFER; 
	//my_print_f("block(%i)",current);
	TCB *t = (TCB*) &threads[current];
	t->sp = *transfer_sp;
	t->pc = *transfer_pc ;
	//my_print_f("GOT pc= %x ; sp= %x \n",transfer_pc,transfer_sp);
	t->state = WAITING;
	t->writeback_buffer = buffer;
	blocked++;

}

void process_unblocking(char c){
	int i;
    for(i=0; i < MAX_THREADS; i++){
        TCB *tcb = (TCB*) &threads[i];
        if (tcb->state==WAITING){
			tcb->state = READY;
			(*tcb->writeback_buffer) = (int) c;
            //my_print_f("UNBLOCKING (%x)\n",i);
			blocked--;
        }
    }
}

void process_sleeping(int time){
	int** transfer_sp = (int**) REG_TRANSFER; 
    thread* transfer_pc = (thread*) PC_TRANSFER; 
	//my_print_f("block(%i)",current);
	
	TCB *t = (TCB*) &threads[current];
	t->sp = *transfer_sp;
	t->pc = *transfer_pc;
	//my_print_f("sp %x %x",*transfer_pc,&t->pc);
	//my_print_f("GOT pc= %x ; sp= %x \n",transfer_pc,transfer_sp);
	t->state = SLEEP;
	t->timer =getTime()+time;
	//my_print_f("%i",current_time+time);
	blocked++;

}

void wake_for_timer(void){
	int i;
	int current_time = getTime();
    for(i=0; i < MAX_THREADS; i++){
        TCB *tcb = (TCB*) &threads[i];
		current_time = getTime();
		if(tcb->state== SLEEP && (tcb->timer<=current_time)){
	        tcb->state=READY;
			blocked--;
			//my_print_f("unblock for Time(%i)\n",i);
		}
    }
}





void schedule(){

    int** transfer_sp = (int**) REG_TRANSFER; 
    thread* transfer_pc = (thread*) PC_TRANSFER; 
	//my_print_f("GOT pc= %x ; sp= %x \n",transfer_pc,transfer_sp);
	TCB *t ;
	isIdle = isIdle || (thread_count==blocked);
    if (isIdle==0 ){
        //my_print_f(">> Save Register\n");
        t= (TCB*) &threads[current];
        t->sp = *transfer_sp;
        t->pc = *transfer_pc;
    } else {
		t = (TCB*) &threads[IDLE_SLOT];
        //t->sp = (int*) USER_STACK_BOTTOM - (MAX_THREADS+1) * STACK_SIZE;;
        //t->pc = idle;
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
		//my_print_f("@%i:",current);
		//my_print_f("pc= %x\n",t->sp);
        if (current != old){
            my_print_f("\n");
		}
    }else{
		//my_print_f("@%i:",IDLE_SLOT);
        my_print_f("IDLE!\n");
		t = (TCB*) &threads[IDLE_SLOT];
    }
	
	//my_print_f("CONTINUE pc= %x ; sp= %x \n",t->pc,t->sp);
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
    int next = find_slot();
    if (next == -1){
        my_print_f("[Exception] No empty Thread slot available\n");
        return -1;
    }

	TCB *tcb = (TCB*) &threads[next];
	tcb->pc = t;
	tcb-> id = ++count;
	tcb->writeback_buffer = &dc;
	tcb->timer = 0;
    int* stack_bottom = (int*) USER_STACK_BOTTOM - next * STACK_SIZE;
	//my_print_f("stackbtm %x",stack_bottom);
    *stack_bottom = param;
    *(stack_bottom-4) = param;
	tcb->sp = stack_bottom;
	tcb-> state = READY;
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
    for(;;){/*my_print_f("F");*/};
}


void init_scheduler(){
    int i;
    for(i=0; i < MAX_THREADS+1; i++){
        TCB *tcb = (TCB*) &threads[i];
        tcb->state=DEAD;
        tcb->id=0;
    }
    TCB *tcb = (TCB*) &threads[IDLE_SLOT];
	tcb->sp = (int*) USER_STACK_BOTTOM - (IDLE_SLOT) * STACK_SIZE;;
	tcb->pc = idle;

	int** transfer_sp = (int**) REG_TRANSFER; 
    thread* transfer_pc = (thread*) PC_TRANSFER; 
	
	*transfer_sp = tcb->sp;
    *transfer_pc = tcb->pc;
}

