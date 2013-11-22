/* 
 * hijack.c: install and resume handlers for swi and irq 
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/09/2013
 */

//#include <exports.h>

//#include <arm/exception.h>
#include <hijackConst.h>
//#include <exit_sys_asm.h>


int install_handler();				// "Wire in" my own SWI handler
int install_irqhandler();



// hijack the Uboot's swi handler
int install_handler() {
	int *swi_entry = (int *)VECTORTABLE_SWI_ENTRY;			// swi vector

	if ((((*swi_entry) ^ LOAD_PC_PC_PLUS_OFFSET_MASK) & 0xfffff000) == 0) {		// check the instruction at swi vector to see if it's legal 
	} else if ((((*swi_entry) ^ LOAD_PC_PC_MINUS_OFFSET_MASK) & 0xfffff000) == 0) {
	} else {
		return 0x0badc0de;					// if illegal, exit with status 0x0badc0de
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

	return 0;

}



// hijack the Uboot's IRQ handler
int install_irqhandler() {
	int *swi_entry = (int *)VECTORTABLE_IRQ_ENTRY;			// IRQ vector

	if ((((*swi_entry) ^ LOAD_PC_PC_PLUS_OFFSET_MASK) & 0xfffff000) == 0) {		// check the instruction at IRQ vector to see if it's legal 
	} else if ((((*swi_entry) ^ LOAD_PC_PC_MINUS_OFFSET_MASK) & 0xfffff000) == 0) {
	} else {
		return 0x0badc0de;					// if illegal, exit with status 0x0badc0de
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
	
	return 0;
}

