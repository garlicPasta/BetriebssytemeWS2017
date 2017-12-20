#ifndef _SHEUDELER_H_
#define _SHEUDELER_H_

#include <threading.h>
static int SAVED_REGISTER[16];

void sheudele();
void add(thread t, int param);
void finish();

#endif
