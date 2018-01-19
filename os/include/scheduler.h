#ifndef _SHEUDELER_H_
#define _SHEUDELER_H_

#include <threading.h>

void schedule();
int add(thread t, int param);
int remove(int id);
void finish();
void wake_for_timer(void);
void process_sleeping(int time);
void process_blocking(int* buffer);
void process_unblocking(char c);

#endif
