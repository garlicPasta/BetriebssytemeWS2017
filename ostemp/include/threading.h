#ifndef _THREADING_H_
#define _THREADING_H_

#define REGISTER_COUNT 15

typedef void (*thread)(int c);
enum tstate {DEAD,READY,RUNNING,WAITING,WAITING_FOR_CHAR,WAITING_FOR_TIMER};
typedef enum tstate thread_state ;
typedef struct{
	int id;
	thread_state state;
	//
	int wait_end;
	// 
	int* sp;
	thread pc;
	
} TCB;

int start_thread(thread r, int param);
int kill_thread(int id);


#endif
