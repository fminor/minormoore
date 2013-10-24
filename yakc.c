/* Filename: yakc.c
 * Dayton Minor
 * Jared Moore
 * Contains kernel c code
 * Lab 4c
 *
 * */

#include "clib.h"
#include "yakk.h"
/* Global Variables */

int YKCtxSwCount; /* Global variable tracking context switches */
int YKIdleCount; /* Global variable used by idle task */
int YKTickNum; /* Global variable incremented by tick handler */
int YKISRDepth;

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

void printList(TCBptr list) {
	TCBptr tcb = list;
	int safety = 0;
	printString("Active tasks: ");
	printInt(activeTasks);
	while(tcb != NULL && safety < activeTasks) {
		printString(" Task P: ");
		printInt(tcb->priority);
		tcb = tcb->next;
		safety++;
	}
	printNewLine();	
}

//1
void YKInitialize() { /* Initializes all required kernel data structures */
	int i;
	TCBptr tempList;
	printString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	activeTasks = 0;
	YKCtxSwCount = 0;
	YKIdleCount = 0;
	YKTickNum = 0;
	/* Set up TCB Lists */
	YKRdyList = NULL;
	YKSuspList = NULL;
	runningTask = NULL;
	YKAvailTCBList = YKTCBArray;
	YKTCBArray[0].prev = NULL;
	for(i = 0; i < MAXTASKS - 1; i++){ /* put the YKTCBArray into the available TCB list */ // this doesn't work
		YKTCBArray[i].next = &YKTCBArray[i + 1];
		YKTCBArray[i + 1].prev = &YKTCBArray[i];
	}
	// Creates idle task
	YKNewTask(YKIdle, (void*)&IStk[ISTACKSIZE],255);
	printList(YKRdyList);
	//printString("\ninitialized\n");
}

//2
void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) { /* Creates a new task */
	int ip,sp;
	// Need to write the assembly function	
	//TCBptr new_task = dequeue(YKAvailTCBList);
	TCBptr new_task = &YKTCBArray[activeTasks];
	printList(YKRdyList);
//	printString("newTask\n\r");
	activeTasks++;
	new_task->priority = priority;
	new_task->state = READY;
	new_task->delay = 0;
	new_task->next = NULL;
	new_task->prev = NULL;
	YKRdyList = queue(YKRdyList,new_task);
	printList(YKRdyList);
	ip = (int) task & 0xFFFF;
	sp = (int) taskStack & 0xFFFF;
	printString("Task priority: \n");
	printInt(new_task->priority);
	printNewLine();
	printString("Task stack: \n");
	printInt(sp);
	printNewLine();
	sp = initStack(ip,sp);
	new_task->sp = (void*)sp;
	printString("New Stack: \n");
	printInt(sp);
	printNewLine();
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
	printList(YKRdyList);
	YKScheduler();
}

void YKDelayTask(int count) { /* Delays a task for specified number of clock ticks*/
	//TCBptr task = remove(YKRdyList,task);
	printString("Delayed Task: ");
	printInt(runningTask->priority);
	printNewLine();
	runningTask->delay=count;
	printString("Delay for: ");
	printInt(runningTask->delay);
	printNewLine();
	//
	suspendTask(runningTask);
	printString("Top of the Suspended List: ");
	printInt(YKSuspList->priority);
	printNewLine();
	printString("Delayed for: ");
	printInt(YKSuspList->delay);
	printNewLine();
	YKScheduler();
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


//2.5
void YKScheduler() { /* Determines the highest priority ready task */
	TCBptr next;
	printString("Scheduler\n\r");
//	printString("Next task priority before dequeue: ");
//	printInt(YKRdyList->priority);
//	printNewLine();
	next = peak(YKRdyList);
	printString("Ready tasks: ");
	printList(YKRdyList);
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
	//if((runningTask != NULL) && (next->priority > runningTask->priority)){
	//	printString("Branch 1\n");
	//	YKRdyList = queue(YKRdyList,next);
//	}else {
	//	printString("Branch 2\n");
	printString("Next task priority: ");
	printInt(next->priority);
	printNewLine();
	//printString("\n
	if(runningTask != NULL && runningTask->priority != next->priority){
		printString("Context Switch\n");
		YKCtxSwCount++;
	}
	//YKRdyList = queue(YKRdyList,runningTask);
	YKDispatcher(next);
//	}
}

//2.5
void YKDispatcher(TCBptr next) { /* Begins or resumes execution of the next task */
	void* sp;
	printString("Dispatcher\n\r");
	runningTask = next;
	sp = next->sp;
	printString("Stack: ");
	printInt((int)sp);
	printNewLine();
	next->state = RUNNING;
	//YKEnterMutex();
	dispatchTask(sp);
	//YKExitMutex();
}

void YKTickHandler() { /* The kernel's timer tick interrupt handler */
	//print Tick information
	printNewLine();
	printString("Tick ");
	printInt(YKTickNum);
	printNewLine();
	//update tick info
	YKTickNum++;


	//decrement top of YKSuspList->delay
	if(YKSuspList != NULL) {
		YKSuspList->delay = YKSuspList->delay - 1;
		printString("Top delay: ");
		printInt(YKSuspList->delay);
		printNewLine();
		while(YKSuspList!= NULL && YKSuspList->delay == 0){
			TCBptr task = dequeue(&YKSuspList);	/* get the top of the list */	
		//	YKSuspList = task->next;		/* update the suspended list */
		//	task->next = NULL;			/* clean up the task */
			YKRdyList = queue(YKRdyList,task);	/* put the task on the queue */
			printInt(YKSuspList->priority);
		}
		printString("Next ready task priority: ");
		printInt(YKRdyList->priority);
		printNewLine();
	}
}

void YKIdle() { /* Idle task for the kernel */
	int i;
	while(1) {
		YKIdleCount++;
		i = 0;
		i = 0;
		//for(i = 0; i < 500; i++);
		//printString("Idle geese\n");
	}
}

TCBptr peak(TCBptr head){
	return head;
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

TCBptr dequeue(TCBptr* queue_head){
	TCBptr next = *queue_head;
	*queue_head = (*queue_head)->next;		//does nothing
	(*queue_head)->prev = NULL;
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
	printList(YKRdyList);
	printString("suspend task (current list)\n");
	printList(YKSuspList);
	task->state = DELAYED;
	printString("task delay: ");
	printInt(task->delay);
	printString(" task prioirty: ");
	printInt(task->priority);
	printNewLine();
	printString("YKSuspList: ");
	printInt((int)YKSuspList);
	printNewLine();
	printString("Top delay: ");
	printInt(YKSuspList->delay);
	printNewLine();
	temp = dequeue(&YKRdyList);
	if(temp->priority == task->priority) {
		task = temp;
	} else {
		printString("ERROR IN SUSPEND TASK");
	}
	if(YKSuspList == NULL){
		printString("first suspended task\n");
		YKSuspList = task;
	}
	else if(task->delay < YKSuspList->delay){
		printString("task to front of queue\n");
		task->next = YKSuspList;
		task->prev = NULL;
		YKSuspList->delay -= task->delay;
                YKSuspList = task;
	} else { /* Needs fixin later*/
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
	// Only time this should be called is when the top priority task is delaying itself
/*	if(YKRdyList != NULL && YKRdyList->priority == task->priority){
		printString("Ready list: ");
		printList(YKRdyList);
		printString("Delayed task priority: ");
		printInt(YKRdyList->priority);
		dequeue(&YKRdyList); 
		printString(". Next ready task: ");
		printInt(YKRdyList->priority);
		printNewLine();
	}*/
}

void saveStack(void* sp){
	runningTask->sp = sp;
}


	
	
