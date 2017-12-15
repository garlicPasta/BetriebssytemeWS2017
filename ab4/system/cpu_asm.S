#include "memlayout.h"

.global _set_stack
_set_stack:
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, r0
  	msr	cpsr_c, r3

 	/* Stack setzen */
  	mov	sp, r1

 	/* Modus mittels r2 zurücksetzen */ 
  	msr	cpsr_c, r2
  	mov	pc, lr


undef_trampoline:
 	/* Debug-Registersatz sichern */ 
 	stmdb	sp, {r0-r15}
 	sub	sp, sp, #(16*4)
 	/* C-Handler (ohne Wiederkehr) aufrufen mit Zeiger auf Registersatz */ 
  	mov	r0, sp
 	B	undef_handler

.global _enable_interrupts
_enable_interrupts:
  	mrs	r2, cpsr
 	bic	r3, r2, #0x80
  	msr	cpsr, r3
  	mov	pc, lr
.end
