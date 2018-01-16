#ifndef _SHEUDELER_H_
#define _SHEUDELER_H_

#include <threading.h>

void schedule();
int add(thread t, int param);
int remove(int id);
void finish();
void block_current_on_char();
void block_current_on_timer(int time);
void wake_for_char();
void wake_for_timer();

#endif
