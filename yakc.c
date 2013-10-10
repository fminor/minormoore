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

int activeTasks;

TCBptr runningTask;		/* the task currently running */
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
	activeTasks = 0;
	YKCtxSwCount = 0;
	YKIdleCount = 0;
	YKTickNum = 0;
	/* Set up TCB Lists */
	YKRdyList = NULL;
	YKSuspList = NULL;
	YKAvailTCBList = YKTCBArray;
	YKTCBArray[0].prev = NULL;
	for(i = 0; i < MAXTASKS + 1; i++){ /* put the YKTCBArray into the available TCB list */ // this doesn't work
		YKTCBArray[i].next = &YKTCBArray[i + 1];
		YKTCBArray[i + 1].prev = &YKTCBArray[i];
	}
	// Creates idle task
	YKNewTask(YKIdle, (void*)&IStk[ISTACKSIZE],255);
//	printString("\n\n\n\ninitialized\n");
}

//2
void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) { /* Creates a new task */
	int ip,sp;
	// Need to write the assembly function	
	//TCBptr new_task = dequeue(YKAvailTCBList);
	TCBptr new_task = &YKTCBArray[activeTasks];
//	printString("newTask\n\r");
	activeTasks++;
	new_task->priority = priority;
	new_task->state = READY;
	new_task->delay = 0;
	YKRdyList = queue(YKRdyList,new_task);
	ip = (int) task & 0xFFFF;
	sp = (int) taskStack & 0xFFFF;
	sp = initStack(ip,sp);
	new_task->sp = (void*)sp;
	//For debugging
//	printString("Creating task of priority ");
//	printInt(priority);
//	printNewLine();
//	printString("Active tasks: ");
//	printInt(activeTasks);
//	printNewLine();	
	if(runningTask != NULL){
		YKScheduler();
	}

}



//3
void YKRun() { /* Starts actual execution of user code */
//	printString("run\n\r");
	YKScheduler();
}

void YKDelayTask(int count) { /* Delays a task for specified number of clock ticks*/
	//TCBptr task = remove(YKRdyList,task);
	runningTask->delay=count;
	suspendTask(runningTask);
}

//void YKEnterMutex() { /* Disables interrupts */
	//get flags
	//0 out interrupt bit
	//save flags
//}

//void YKExitMutex() { /* Enables interrupts */
	//get flags
	//or the interrupt bit
	//save flags
//}

/* These two should be written in assembly */
void YKEnterISR() { /* Called on entry to ISR */

}

void YKExitISR() { /* Called on exit from ISR */

}

//2.5
void YKScheduler() { /* Determines the highest priority ready task */
	TCBptr next;
//	printString("Scheduler\n\r");
//	printString("Next task priority before dequeue: ");
//	printInt(YKRdyList->priority);
//	printNewLine();
	next = dequeue(YKRdyList);
//	printString("Next task priority after dequeue: ");
//	printInt(YKRdyList->priority);
	//printNewLine();
//	if(runningTask != NULL) 
//		printString("runningTask is not NULL\n");
//	else
	//	printString("runningTask is NULL\n");
//	if(next->priority != NULL && runningTask!=NULL) {
	//	printInt(next->priority);
	//	printNewLine();
	//	if(next->prev != NULL)
	//		printString("Next task has a previous task");
	//	else
	//		printString("next->prev = NULL");
	//	printNewLine();
	//	printInt(runningTask->priority);
	//	printNewLine();
//	}
	if((runningTask != NULL) && (next->priority > runningTask->priority)){
	//	printString("Branch 1\n");
		YKRdyList = queue(YKRdyList,next);
	}else {
	//	printString("Branch 2\n");
		YKCtxSwCount++;
		YKRdyList = queue(YKRdyList,runningTask);
		YKDispatcher(next);
	}
}

//2.5
void YKDispatcher(TCBptr next) { /* Begins or resumes execution of the next task */
	void* sp;
//	printString("Dispatcher\n\r");
	runningTask = next;
	sp = next->sp;
	next->state = RUNNING;
	YKEnterMutex();
	dispatchTask(sp);
	YKExitMutex();
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
	int i;
	while(1) {
		YKIdleCount++;
	//	for(i = 0; i < 500; i++) {
	//		printString("Idle geese");
	//	}
	}
}

TCBptr queue(TCBptr queue_head, TCBptr task){
	if(queue_head == NULL){
		return task;
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
//	printString("Next task priority :");
//	printInt(next->priority);
//	printNewLine();
//	printString("Next next task priorty: ");
//	printInt((*queue_head)->priority);
//	printNewLine();
	return next;
}

void suspendTask(TCBptr task){
	TCBptr temp = NULL;
	task->state = DELAYED;
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
	
	
