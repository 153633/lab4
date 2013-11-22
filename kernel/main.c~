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

		
	assert(0);        /* should never get here */
}


