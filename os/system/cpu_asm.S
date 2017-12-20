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

.global _switch_system_mode
_switch_system_mode:
    mov r5, lr
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, #0x1f
  	msr	cpsr_c, r3
    bx r5

.global _switch_interrupt_mode
_switch_interrupt_mode:
    mov r5, lr
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, #0x12
  	msr	cpsr_c, r3
    bx r5

.global _tramp_irq
_tramp_irq:
    sub lr, lr, #4
    push {r0,lr}
    bl has_received_char
    cmp r0, #1
    BLEQ _tramp_exit_irq

    pop {r0}
    bl _switch_system_mode
    push {r0-r12,LR}
    ldr r0,=REG_TRANSFER
    stm r0,{sp}
    bl _switch_interrupt_mode
    pop {lr}
    ldr r0,=PC_TRANSFER
    stm r0,{lr}
    pop {lr}
    bl irq_handler
    bl _switch_system_mode
  	mrs	r2, cpsr
 	bic	r3, r2, #0x80
  	msr	cpsr, r3
    ldr R9,=REG_TRANSFER
    ldr SP,[R9]
    pop {r0-r12,LR}
    ldr R9,=PC_TRANSFER
    ldr PC,[R9]

.global _tramp_exit_irq
_tramp_exit_irq:
    bl irq_handler_dbgu
    ldm sp!, {r0,pc}^

.end
