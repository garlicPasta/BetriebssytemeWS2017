#ifndef _THREADING_H_
#define _THREADING_H_

typedef void (*run_method)() ;
void start_thread(run_method r);

#endif
