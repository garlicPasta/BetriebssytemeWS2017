#ifndef _SHEUDELER_H_
#define _SHEUDELER_H_

enum thread_state {running,dead};

typedef struct{
	int pc;
	int registers[15];
	//registers ?!
	thread_state;

	} TCB;

#endif
