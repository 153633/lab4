/* 
 * sys.c: handling system calls. 
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/20/2013
 */

#include <bits/swi.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <syscall.h>

int getSWI(int num, int *sp) {
	switch(num) {
		case READ_SWI:			
			return read_syscall((int) *sp, (char *) *(sp+1), (unsigned int) *(sp+2));
		case WRITE_SWI:
			return write_syscall((int) *sp, (char *) *(sp+1), (unsigned int) *(sp+2));
		case TIME_SWI:
			return time_syscall();
		case SLEEP_SWI:
			sleep_syscall((unsigned int) *sp);
			return 0; 
		default:
			break; //tanke into account invalid syscalls
	}
	return 0;
}
