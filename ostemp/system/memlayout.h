#ifndef _MEMLAYOUT_H_
#define _MEMLAYOUT_H_

#define STACK_BOTTOM 0x24000000
#define REG_TRANSFER 0x21000000
#define PC_TRANSFER 0x21000004
#define USER_STACK_BOTTOM (STACK_BOTTOM - 8 * STACK_SIZE)
#define STACK_SIZE  (1024*64)

#endif
