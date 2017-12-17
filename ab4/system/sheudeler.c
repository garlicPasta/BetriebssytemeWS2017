#include <threading.h>
#include <sheudeler.h> //? automatisch?
#define MAX_THREADS 16
#define PC 15
#include "my_print.h"
int current = 0;
int next = 0;
static int count = 0;
static TCB threads[MAX_THREADS];

void sheudele(){
	my_print_f("start scheudele\n");
	int tried = 0;
	int found = 0;
	TCB *t ;
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
		my_print_f("Found one!\n");
		//asm("b %0":"r"(t->registers[PC]));
		((run_method) t->registers[PC])();
	}else{
		my_print_f("IDLE!\n");
	}
}

void add(run_method r){
	int i;
	TCB *t = (TCB*) &threads[next];
	t-> id = ++count;
	t-> state = READY;
	for(i=0;i<REGISTER_COUNT; i++){
		t->registers[i] = 0; // copy current?
	}
	t->registers[PC] = r;
	//pc == r ?
	//t->registers[0] = ;
	next = (next+1)% MAX_THREADS;
	
}

void finish(){
	TCB * t = &threads[current];
	t->state = DEAD;
	//scheudele();
}
