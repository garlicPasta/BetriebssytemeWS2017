#include <threading.h>
#include <scheduler.h>

int start_thread(thread t, int param){
	return add(t, param);
}

int kill_thread(int id){
	return remove(id);
}
