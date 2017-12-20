#ifndef _THREADING_H_
#define _THREADING_H_

#define REGISTER_COUNT 15

typedef void (*thread)(int c);
enum tstate {DEAD,READY,RUNNING,WAITING};
typedef enum tstate thread_state ;
typedef struct{
	int id;
	thread_state state;
	int param ;
	int sp;
	int pc;
	
} TCB;

void start_thread(thread r, int param);
void end_thread();

#endif
