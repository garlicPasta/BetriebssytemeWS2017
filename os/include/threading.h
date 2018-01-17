#ifndef _THREADING_H_
#define _THREADING_H_

#define REGISTER_COUNT 15

typedef void (*thread)(int c);
enum tstate {DEAD,READY,RUNNING,WAITING, SLEEP};
typedef enum tstate thread_state ;
typedef struct{
	int id;
	thread_state state;
	int* sp;
	thread pc;
	int* writeback_buffer;
	
} TCB;

int start_thread(thread r, int param);
int kill_thread(int id);

#endif
