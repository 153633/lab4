
/* 
 * hijack.h: install and resume handlers for swi and irq 
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/09/2013
 */

#ifndef HIJACK_H
#define HIJACK_H

extern void install_handler();				// "Wire in" my own SWI handler
extern void resume_handler();				// Resume the two instructions of the old SWI handler

extern void setup();
extern void install_irqhandler();
extern void IRQ_Handler();
extern void C_IRQ_Handler();


#endif
