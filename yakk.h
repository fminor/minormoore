/* Filename: yakk.h
 * Dayton Minor
 * Jared Moore
 * Contains kernel stuff
 * Lab 4c
 *
 * */

#include "yaku.h"

#define NULL 0

/* Define states */
#define READY 0
#define RUNNING 1
#define DELAYED 2
#define SUSPENDED 3


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

typedef struct {
	int ID;
	int value;
} YKSEM;

typedef struct queueNode * QMSGptr;
//typedef struct queueNode {
//	void* msg;
//	QMSGptr next;
//} QMSG;
typedef struct queueblock {
	void** queue;
	int head;
	int tail;
	//int count;
	unsigned length;
} YKQ;

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
void YKEnterISR(); /* Called on entry to ISR */
void YKExitISR(); /* Called on exit from ISR */
void YKScheduler(int); /* Determines the highest priority ready task and gives to dispatcher */
void YKDispatcher(TCBptr); /* Begins or resumes execution of the next task */
void YKTickHandler(); /* The kernel's timer tick interrupt handler */

/* Semaphore functions */
YKSEM* YKSemCreate(int);
void YKSemPend(YKSEM*);
void YKSemPost(YKSEM*);
/* Queue functions */
YKQ *YKQCreate(void **start, unsigned size);
void *YKQPend(YKQ *queue);
int YKQPost(YKQ *queue, void *msg);

/* Custom Kernel Functions */
void YKEnterMutex(); /* Disables interrupts */
void YKExitMutex(); /* Enables interrupts */
void YKIdle(); /* Idle task for the kernel */
int initStack(int, int); /* Assembely Function that initializes stack for new functions */
TCBptr queue(TCBptr, TCBptr); /* Puts task onto prioritized queue */
TCBptr dequeue(TCBptr*); /* Removes head of queue, returns removed node */
TCBptr peak(TCBptr); /*Returns head of queue */
void suspendTask(TCBptr); /* Puts onto delayed queue */
void dispatchTask(void* ); 
void YKSaveContext();


/* Global Variables */

extern int YKCtxSwCount; /* Global variable tracking context switches */
extern int YKIdleCount; /* Global variable used by idle task */
extern int YKTickNum; /* Global variable incremented by tick handler */



