#include <threading.h>
#include <scheduler.h>


void start_thread(thread t, int param){
	add(t, param);
}

