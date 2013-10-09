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
int IStk[ISTACKSIZE];
/* Function declarations */

//1
void YKInitialize() { /* Initializes all required kernel data structures */
	int i;
	TCBptr tempList;
	YKCtxSwCount = 0;
	YKIdleCount = 0;
	YKTickNum = 0;
	/* Set up TCB Lists */
	YKRdyList = NULL;
	YKSuspList = NULL;
	YKAvailTCBList = YKTCBArray;
	YKTCBArray[0].prev = NULL;
	for(i = 0; i < MAXTASKS + 1; i++){ /* put the YKTCBArray into the available TCB list */
		YKTCBArray[i].next = &YKTCBArray[i + 1];
		YKTCBArray[i + 1].prev = &YKTCBArray[i];
	}
	// Creates idle task
	YKNewTask(YKIdle, (void*)&IStk[ISTACKSIZE],255);
	
}

//2
void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) { /* Creates a new task */
	int cs,ip,ss,sp;
	// Need to write the assembly function	
	TCBptr new_task = YKAvailTCBList++;
	new_task->priority = priority;
	new_task->state = READY;
	new_task->sp = taskStack;
	new_task->delay = 0;
	YKRdyList = queue(YKRdyList,new_task);
	ip = (int) task & 0xFFFF;
	cs = 0;
	sp = (int) taskStack & 0xFFFF;
	ss = 0;
	initStack(cs,ip,ss,sp);
	// assembly function:
	// push flags
	// push CS - 0
	// push IP - void (*task)(void)
	// push 0 5 times
	//
}



//3
void YKRun() { /* Starts actual execution of user code */
	//call scheduler....
}

void YKDelayTask(int count) { /* Delays a task for specified number of clock ticks*/
	//remove(YKRdyList,task)
	//task->delay=count
	//queue(YkSuspList,task)
}

void YKEnterMutex() { /* Disables interrupts */
	//get flags
	//0 out interrupt bit
	//save flags
}

void YKExitMutex() { /* Enables interrupts */
	//get flags
	//or the interrupt bit
	//save flags
}

/* These two should be written in assembly */
void YKEnterISR() { /* Called on entry to ISR */

}

void YKExitISR() { /* Called on exit from ISR */

}

//2.5
void YKScheduler() { /* Determines the highest priority ready task */
	TCBptr next = dequeue(YKRdyList);
	YKDispatcher(next);
}

//2.5
void YKDispatcher(TCBptr next) { /* Begins or resumes execution of the next task */
	//switch out sp
	//pop dx
	//pop cx
	//pop bx
	//pop ax
	//pop something else
	//iret
}

void YKTickHandler() { /* The kernel's timer tick interrupt handler */
	//decrement top of YKSuspList->delay
	YKSuspList->delay = YKSuspList->delay - 1;
	if(YKSuspList->delay == 0){
		TCBptr task = dequeue(YKSuspList);
		YKRdyList = queue(YKRdyList,task);
	}
}

void YKIdle() { /* Idle task for the kernel */
	while(1);
}

TCBptr queue(TCBptr queue_head, TCBptr task){
	if(queue_head == NULL){
		return queue_head;
	}
	if(queue_head->priority > task->priority){
		task->next = queue_head;
		queue_head->prev = task;
		queue_head = task;
	} else {
		TCBptr temp = queue_head;
		while(temp->next->priority < task->priority) {
			temp = temp->next;
		}
		task->next = temp->next;
		task->next->prev = task;
		task->prev = temp;
		temp->next = task;
	}
	return queue_head;
}

TCBptr dequeue(TCBptr queue_head){
	TCBptr next = queue_head;
	queue_head = queue_head->next;
	return next;
}

void suspendTask(TCBptr task){
	TCBptr temp = NULL;
	if(task->delay < YKSuspList->delay){
		YKSuspList->delay -= task->delay;
                YKSuspList = task;
	} else {
		//task->delay -= YKSuspList->delay;
		//temp = YKSuspList->next;
		temp = YKSuspList;
		while(temp->next != NULL && temp->delay < task->delay){
			task->delay -= temp->delay;
			temp = temp->next;
		}
		if(temp->delay < task->delay) {
			task->delay -= temp->delay;
			temp->next = task;
			task->prev = temp;
		} else {
			task->prev = temp->prev;
			task->next = temp;
			temp->prev = task;
			temp->delay -= task->delay;
		}
	}
}
	
	
