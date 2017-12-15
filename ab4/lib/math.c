#include <debug_unit.h>

int fibo(int num){
	if(num < 2){return 1;}
	return fibo(num-2)+fibo(num-1);
}
