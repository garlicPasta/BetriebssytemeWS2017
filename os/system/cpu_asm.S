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

.global _disable_interrupts
_disable_interrupts:
  	mrs	r2, cpsr
 	orr	r3, r3, #0x80
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

.global _switch_user_mode
_switch_user_mode:
    mov r5, lr
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, #0x10
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

.global _switch_svc_mode
_switch_svc_mode:
    mov r5, lr
  	mrs	r2, cpsr
 	bic	r3, r2, #0x1f
 	orr	r3, r3, #0x13
  	msr	cpsr_c, r3
    bx r5

.global _tramp_irq
_tramp_irq:
    sub lr, lr, #4
    push {r0,lr}
    bl has_received_char
    cmp r0, #1
    bleq _tramp_exit_irq

    pop {r0}
    bl _switch_system_mode
    push {r0-r12,LR}
    // store addres of REG_TRANSER in RO
    ldr r0,=REG_TRANSFER
    // store SP at REG_TRANSFER
    stm r0,{sp}
    bl _switch_interrupt_mode
    pop {lr}
    ldr r0,=PC_TRANSFER
    stm r0,{lr}
    bl irq_handler
    bl _switch_system_mode
    bl _enable_interrupts
    b _switch_thread

_switch_thread:
    ldr R9,=REG_TRANSFER
    ldr SP,[R9]
    bl _switch_user_mode
    pop {r0-r12,LR}
    ldr R9, =PC_TRANSFER
    ldm R9, {pc}

.global _tramp_exit_irq
_tramp_exit_irq:
    bl irq_handler_dbgu
    ldm sp!, {r0,pc}^

.global _tramp_swi
_tramp_swi:
    push {lr,r0}
    bl _switch_system_mode
    push {r0-r12,LR}
    ldr r0,=REG_TRANSFER
    stm r0,{sp}
    bl _switch_svc_mode
	pop {r1,lr}
    ldr r0,=PC_TRANSFER
    stm r0,{lr}
	ldr	r0,[lr,#-4]
	bic	%r0,r0,#0xff000000
    bl swi_handler
    bl _switch_system_mode
    bl _enable_interrupts
    b _switch_thread

.global _set_page_table
_set_page_table:
    ldr r0,=PAGE_TABLE_BASE
    MCR p15, 0, R0, C2, C0, 0
    MOV R0, #0xffffffff
    MCR p15, 0, R0, C3, C0, 0
    bx lr

.global _enable_mmu
_enable_mmu:
    MCR p15, 0, R1, C1, C0, 0
    ORR R1, #0x1
    MCR p15, 0, R1, C1, C0, 0
.end


