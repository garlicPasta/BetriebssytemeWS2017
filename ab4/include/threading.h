#ifndef _THREADING_H_
#define _THREADING_H_

#define REGISTER_COUNT 15

typedef void (*run_method)() ;
enum tstate {DEAD,READY,RUNNING,WAITING};
typedef enum tstate thread_state ;
typedef struct{
	int id;
	thread_state state;
	int registers[REGISTER_COUNT];
	
} TCB;
void start_thread(run_method r);

#endif
