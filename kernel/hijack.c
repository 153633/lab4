/* 
 * hijack.c: install and resume handlers for swi and irq 
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/09/2013
 */

#include <exports.h>

//#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>

#include <hijackConst.h>
//#include <exit_sys_asm.h>
#include <systime.h>

void install_handler();				// "Wire in" my own SWI handler
void resume_handler();				// Resume the two instructions of the old SWI handler

void setup();
void install_irqhandler();
void IRQ_Handler();
void C_IRQ_Handler();


// hijack the Uboot's swi handler
void install_handler() {
	int *swi_entry = (int *)VECTORTABLE_SWI_ENTRY;			// swi vector

	if ((((*swi_entry) ^ LOAD_PC_PC_PLUS_OFFSET_MASK) & 0xfffff000) == 0) {		// check the instruction at swi vector to see if it's legal 
		printf("Correct, positive offset.\n");
	} else if ((((*swi_entry) ^ LOAD_PC_PC_MINUS_OFFSET_MASK) & 0xfffff000) == 0) {
		printf("Correct, negative offset.\n");
	} else {
		//exit_sys_asm(0x0badc0de);					// if illegal, exit with status 0x0badc0de
	}
	
	int offset = (*swi_entry) & 0x0fff;
	int jump_entry = VECTORTABLE_SWI_ENTRY + PC_CURRENTADDR_OFFSET + offset;	// address of jump table entry

	int *iaddrptr = *((int**)jump_entry);				// address of uboot's swi hander instruction

	iaddr_1 = (int *) iaddrptr;					// store the address and content of uboot's swi handler instruction 
	iaddr_2 = (int *) (iaddrptr + 1);
	instruction1 = *iaddr_1;
	instruction2 = *iaddr_2;
	
	*iaddrptr = LOAD_PC_PC_4_ENCODING;				// hijack using 'ldr pc, [pc, #-4]'
	*(iaddrptr+1) = (int)S_Handler;					// hijsck using address of our swi handler

}

// restore the uboot's swi handler instructions 
void resume_handler() {
	*iaddr_1 = instruction1;
	*iaddr_2 = instruction2;
	*irq_iaddr_1 = irq_instruction1;
	*irq_iaddr_2 = irq_instruction2;
}

// Setup for the interrupt controller and timer
void setup() {
	// Setup the interrupt controller
	reg_write(INT_ICMR_ADDR, 0x04000000);		// only enable the interrupt of os timer0
	reg_write(INT_ICLR_ADDR, 0);			// set all interrupt to IRQ

	// Setup the timer
	reg_write(OSTMR_OSCR_ADDR, 0);			// reset the OSCR
	reg_write(OSTMR_OSMR_ADDR(0), FREQ_MS * SYSTIME_PRECISION);		// set the initial OSMR0 to 32500, so it will trigger an interrupt every 10ms

	reg_write(OSTMR_OIER_ADDR, OSTMR_OIER_E0);	// only enable the interrupt of OSMR0
	reg_write(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0);	// clear the bit initially

}

// hijack the Uboot's IRQ handler
void install_irqhandler() {
	int *swi_entry = (int *)VECTORTABLE_IRQ_ENTRY;			// IRQ vector

	if ((((*swi_entry) ^ LOAD_PC_PC_PLUS_OFFSET_MASK) & 0xfffff000) == 0) {		// check the instruction at IRQ vector to see if it's legal 
		printf("Correct, positive offset.\n");
	} else if ((((*swi_entry) ^ LOAD_PC_PC_MINUS_OFFSET_MASK) & 0xfffff000) == 0) {
		printf("Correct, negative offset.\n");
	} else {
		//exit_sys_asm(0x0badc0de);					// if illegal, exit with status 0x0badc0de
	}
	
	int offset = (*swi_entry) & 0x0fff;
	int jump_entry = VECTORTABLE_IRQ_ENTRY + PC_CURRENTADDR_OFFSET + offset;	// address of jump table entry

	int *iaddrptr = *((int**)jump_entry);				// address of uboot's IRQ handler instruction

	irq_iaddr_1 = (int *) iaddrptr;					// store the address and content of uboot's IRQ handler instruction 
	irq_iaddr_2 = (int *) (iaddrptr + 1);
	irq_instruction1 = *irq_iaddr_1;
	irq_instruction2 = *irq_iaddr_2;
	
	*iaddrptr = LOAD_PC_PC_4_ENCODING;				// hijack using 'ldr pc, [pc, #-4]'
	*(iaddrptr+1) = (int)IRQ_Handler;				// hijsck using address of our IRQ handler

}

void C_IRQ_Handler() {
	system_time++;
	reg_write(OSTMR_OSCR_ADDR, 0);			// reset the OSCR
	reg_set(OSTMR_OSSR_ADDR, 1);
}
