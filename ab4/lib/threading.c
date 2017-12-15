#define DEBUG_REG 0xFFFFF200


#include <threading.h>



void start_thread(run_method r){
	r();
}
