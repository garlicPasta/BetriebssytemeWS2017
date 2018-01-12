#ifndef _SHEUDELER_H_
#define _SHEUDELER_H_

#include <threading.h>

void schedule();
int add(thread t, int param);
int remove(int id);
void finish();

#endif
