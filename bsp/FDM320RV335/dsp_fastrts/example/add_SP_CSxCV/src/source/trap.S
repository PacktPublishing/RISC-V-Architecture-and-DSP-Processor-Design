/*
 * trap.S
 *
 */
 /*#include "intexc.h"*/
.section .text
.global trap_init

/*.global PLIC_MCLAIM*/
.align 4

trap_handler:
			/* mstatus.MIE*/
			/* csrsi	mstatus, 0x8 */

			/* 0: exception    */
			j exception_handler
			/* 1: software isr */
			j halt
			/* 2: exit isr     */
			mret
			/* 3: software isr */
			j halt
			/* 4: exit isr     */
			mret
			/* 5: timer isr    */
			j halt
			/* 6: exit isr     */
			mret
			/* 7: timer isr    */
			j halt
			/* 8: exit isr     */
			mret
			/* 9: external isr */
			j external_interrupt_handler
			/* 10: unknown isr */
			mret
			/* 11: external isr*/
			j external_interrupt_handler
			/* 12: exit isr    */
			mret

exception_handler:
			/* handle exception */
			save
			jal exception_handler
			restore
			mret

trap_init:
			/* set trap vector base address */
			la		t1, trap_handler
			/* set mtvec mode 1 */
			ori		t1, t1, 1
			csrw	mtvec, t1
			/* enable nmie bit in mnstatus*/
			csrsi	0x744, 0x8 /* mnstatus reg */
			/* enable MIE in mstauts */
			li t1, 0x7888
			csrw mstatus, t1
			/* csrsi	mstatus, 0x8 */
			/* enable interrupt bits in mie */
			li 		t1, 0x888
			csrrs 	t1, mie, t1
			ret

external_interrupt_handler:

				save			;
				/* put claim address in s1 and IntVectTable in a1*/
				lui	s1, 2048	;
				la	a1, IntVectTable		;
				addi	s1, s1, 432	;
				/* s0 = id, s1 = addr */
				lw s0, 0(s1)		;
				/* write claim back in advance to response next interrupt*/
				sw s0, 0(s1)		;
				/* call isr by id */
				slli a0, s0, 2		;
				add a0, a0, a1		;
				lw a0, 0(a0)		;
				jalr a0			;

				restore			;
				mret			;


halt:
	j halt
