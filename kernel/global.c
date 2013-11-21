// global.c: initiatess all the global variables we will use
//
// Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
//          Zixuan Liu <zixuanl@andrew.cmu.edu>
//	    Jianan Lu <jiananl@andrew.cmu.edu>
// Date:    10/23/2013

#include <types.h>

// user
uint32_t rr8 = 0;

// syscall return
int32_t value = 0 ;

// system time
volatile uint32_t system_time = 0;


