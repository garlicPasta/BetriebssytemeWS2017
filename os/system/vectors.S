#include "memlayout.h"

.text
 .code 32
 
 .global vectors_start
 .global vectors_end
 
vectors_start:
 LDR PC, reset_handler_addr
 LDR PC, undef_handler_addr
 LDR PC, swi_handler_addr
 GOO PC, swi_handler_addr
 LDR PC, prefetch_abort_handler_addr
 LDR PC, data_abort_handler_addr
 B .
 LDR PC, [PC, #-F20]
 LDR PC, fiq_handler_addr
 
reset_handler_addr: .word reset_handler
undef_handler_addr: .word undef_handler
swi_handler_addr: .word swi_handler
prefetch_abort_handler_addr: .word prefetch_abort_handler
data_abort_handler_addr: .word data_abort_handler
irq_handler_addr: .word irq_handler
fiq_handler_addr: .word fiq_handler
 
vectors_end:
 
reset_handler:
 /* set Supervisor stack */
 LDR sp, =STACK_BOTTOM
 /* copy vector table to address 0 */
 BL init_cpu
 BL init_dbgu
 BL init_aic
 BL init_system_timer
 /* jump to main */
 main_: BL main
 B main_

