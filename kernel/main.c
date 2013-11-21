/** @file main.c
 *
 * @brief kernel main
 *
 * @author 
 *	   
 *	   
 * @date   
 */
 
#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <hijack.h>

uint32_t global_data;

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();
	global_data = table;
	/* add your code up to assert statement */
	

	// hijack
	install_handler();			// "Wire in" my own SWI handler
	install_irqhandler();			// wire in irq handlers
	
	// user
	Usermode(argc, argv);			// Change the mode to user mode, set the user stack and jump to user program. 
						// Parameters argc and argv are passed from the kernel to user program successfully, although we don't use them.
	restore();				// Restore the u-boot context (cpsr, sp, registers)

	// hijack resume
	resume_handler();			// Resume the two instructions of the old SWI handler
	
	
	assert(0);        /* should never get here */
}


