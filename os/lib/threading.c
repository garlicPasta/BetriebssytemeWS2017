#include <threading.h>
#include <scheduler.h>

int start_thread(thread t, int param){
	return add(t, param);
}

int destroy_thread(int id){
	return remove(id);
}
