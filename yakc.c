/* Filename: yakc.c
 * Dayton Minor
 * Jared Moore
 * Contains kernel c code
 * Lab 4b
 *
 * */

#include "clib.h"
#include "yakk.h"
/* Global Variables */

int YKCtxSwCount; /* Global variable tracking context switches */
int YKIdleCount; /* Global variable used by idle task */
int YKTickNum; /* Global variable incremented by tick handler */

TCBptr YKRdyList;		/* a list of TCBs of all ready tasks
				   in order of decreasing priority */ 
TCBptr YKSuspList;		/* tasks delayed or suspended */
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs
				   (extra one is for the idle task) */

/* Function declarations */

//1
void YKInitialize() { /* Initializes all required kernel data structures */
	YKCtxSwCount = 0;
	YKIdleCount = 0;
	YKTickNum = 0;
	// Set up TCB Lists
	// YKRdyList
	// YKSuspList
	// YKAvailTCBList
	// Use from YKTCBArray[];
	// Allocate stack space for idle task
	// Creates idle task 
}

//2
void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) { /* Creates a new task */
	// Need to write the assembly function	
}

//3
void YKRun() { /* Starts actual execution of user code */

}

void YKDelayTask(int count) { /* Delays a task for specified number of clock ticks*/

}

void YKEnterMutex() { /* Disables interrupts */

}

void YKExitMutex() { /* Enables interrupts */

}

/* These two should be written in assembly */
void YKEnterISR() { /* Called on entry to ISR */

}

void YKExitISR() { /* Called on exit from ISR */

}

//2.5
void YKScheduler() { /* Determines the highest priority ready task */

}

//2.5
void YKDispatcher() { /* Begins or resumes execution of the next task */

}

void YKTickHandler() { /* The kernel's timer tick interrupt handler */

}


