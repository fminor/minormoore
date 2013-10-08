/* Filename: yakk.h
 * Dayton Minor
 * Jared Moore
 * Contains kernel stuff
 * Lab 4b
 *
 * */

#include "yaku.h"

#define NULL 0

typedef struct taskblock *TCBptr;
typedef struct taskblock { /* the TCB struct definition */
	void* sp;	/* pointer to top of stack */
	int state;	/* current state */
	unsigned char priority;	/* task priority */  
	int delay;	/* # of ticks to wait */
	//int pc;
	TCBptr next;	/* forward ptr for dbl linked list */
	TCBptr prev;	/* backward ptr for dbl linked list */
} TCB;

extern TCBptr YKRdyList;		/* a list of TCBs of all ready tasks
				   in order of decreasing priority */ 
extern TCBptr YKSuspList;		/* tasks delayed or suspended */
extern TCBptr YKAvailTCBList;		/* a list of available TCBs */
extern TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs
				   (extra one is for the idle task) */

/* List code here */

/* Function declarations */

void YKInitialize(); /* Initializes all required kernel data structures */
void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority); /* Creates a new task */
void YKRun(); /* Starts actual execution of user code */
void YKDelayTask(int count); /* Delays a task for specified number of clock ticks*/
void YKEnterMutex(); /* Disables interrupts */
void YKExitMutex(); /* Enables interrupts */
void YKEnterISR(); /* Called on entry to ISR */
void YKExitISR(); /* Called on exit from ISR */
void YKScheduler(); /* Determines the highest priority ready task */
void YKDispatcher(); /* Begins or resumes execution of the next task */
void YKTickHandler(); /* The kernel's timer tick interrupt handler */

/* Global Variables */

extern int YKCtxSwCount; /* Global variable tracking context switches */
extern int YKIdleCount; /* Global variable used by idle task */
extern int YKTickNum; /* Global variable incremented by tick handler */


