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

int debug;

int activeTasks;
int started = 0;

TCBptr runningTask;		/* the task currently running */
TCBptr YKRdyList;		/* a list of TCBs of all ready tasks
				   in order of decreasing priority */ 
TCBptr YKSuspList;		/* tasks delayed or suspended */
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCB    YKTCBArray[MAXTASKS+1];	/* array to allocate all needed TCBs
				   (extra one is for the idle task) */
YKSEM SStack[MAXSEMAPHORES];
TCBptr YKPendList[MAXSEMAPHORES];

YKQ QStack[MAXMESSAGEQUEUES];

int IStk[ISTACKSIZE];
/* Function declarations */

void printList(TCBptr list) {
	TCBptr tcb = list;
	int safety = 0;
//	printString("Active tasks: ");
//	printInt(activeTasks);
	while(tcb != NULL && safety < (activeTasks + 10)) {
		printString(" Task P: ");
		printInt(tcb->priority);
	//	printNewLine();
		tcb = tcb->next;
		safety++;
	}
	printNewLine();	
}

//1
void YKInitialize() { /* Initializes all required kernel data structures */
	int i;
	TCBptr tempList;
//	printString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
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
	for(i = 0; i < MAXTASKS; i++){ /* put the YKTCBArray into the available TCB list */ // this doesn't work
		YKTCBArray[i].next = &YKTCBArray[i + 1];
		YKTCBArray[i + 1].prev = &YKTCBArray[i];
	}
	// Creates idle task
	YKNewTask(YKIdle, (void*)&IStk[ISTACKSIZE],255);
//	printList(YKRdyList);
	//printString("\ninitialized\n");
}

//2
void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) { /* Creates a new task */
	int ip,sp;
	// Need to write the assembly function	
	//TCBptr new_task = dequeue(YKAvailTCBList);
	TCBptr new_task = &YKTCBArray[activeTasks];
//	printString("YKNewTask: ");
//	printList(YKRdyList);
	activeTasks++;
	new_task->priority = priority;
	new_task->state = READY;
	new_task->delay = 0;
	new_task->next = NULL;
	new_task->prev = NULL;
	YKRdyList = queue(YKRdyList,new_task);
//	printList(YKRdyList);
	ip = (int) task & 0xFFFF;
	sp = (int) taskStack & 0xFFFF;
//	printString("Task priority: \n");
//	printInt(new_task->priority);
//	printNewLine();
//	printString("Task stack: \n");
//	printInt(sp);
//	printNewLine();
	sp = initStack(ip,sp);
	new_task->sp = (void*)sp;
//	printString("New Stack: \n");
//	printInt(sp);
//	printNewLine();
	//For debugging
//	printString("Creating task of priority ");
//	printInt(priority);
//	printNewLine();
//	printString("Active tasks: ");
//	printInt(activeTasks);
//	printNewLine();	
	if(runningTask != NULL){
		YKScheduler(0);
	}

}



//3
void YKRun() { /* Starts actual execution of user code */
//	printString("run\n\r");
//	printList(YKRdyList);
	started = 1;
	YKScheduler(1);
}

void YKDelayTask(int count) { /* Delays a task for specified number of clock ticks*/
	YKEnterMutex();
	runningTask->delay=count;
//	printString("Delayed Task: ");
//	printInt(runningTask->priority);
//	printNewLine();
//	printString("Delay for: ");
//	printInt(runningTask->delay);
//	printNewLine();
	suspendTask(runningTask);
//	printString("Top of the Suspended List: ");
//	printInt(YKSuspList->priority);
//	printNewLine();
//	printString("Delayed for: ");
//	printInt(YKSuspList->delay);
//	printNewLine();
	YKScheduler(0);
	YKExitMutex();
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
void YKScheduler(int contextSaved) { /* Determines the highest priority ready task */
	TCBptr next;	
	if(!started)
		return;
	next = peak(YKRdyList);
	if(runningTask != NULL && runningTask->priority != next->priority){
		YKEnterMutex();
		YKCtxSwCount++;
		if(contextSaved ==  0) {
			YKSaveContext();
		}
		YKDispatcher(next);
	} else if (runningTask == NULL){
		YKDispatcher(next);
	}
}

//2.5
void YKDispatcher(TCBptr next) { /* Begins or resumes execution of the next task */
	void* sp;
//	printString("Dispatcher\n\r");
	runningTask = next;
	sp = next->sp;
//	printString("Stack: ");
//	printInt((int)sp);
//	printNewLine();
	next->state = RUNNING;
	//YKEnterMutex();
	dispatchTask(sp);
	//YKExitMutex();
}

void YKTickHandler() { /* The kernel's timer tick interrupt handler */
	//print Tick information
/*	printNewLine();
	printString("Tick ");
	printInt(YKTickNum);
	printNewLine(); */
	//update tick info
	YKTickNum++;
	while(YKTickNum > 20);

	//decrement top of YKSuspList->delay
	if(YKSuspList != NULL) {
		YKEnterMutex();
		YKSuspList->delay = YKSuspList->delay - 1;
//		printString("Top delay: ");
//		printInt(YKSuspList->delay);
//		printNewLine();
//		printString("Tick/susp Ready List ");
//		printList(YKRdyList);
//		printNewLine();
//		printList(YKSuspList);
		while(YKSuspList!= NULL && YKSuspList->delay == 0){
			TCBptr task = dequeue(&YKSuspList);	/* get the top of the list */	
		//	YKSuspList = task->next;		/* update the suspended list */
		//	task->next = NULL;			/* clean up the task */
			YKRdyList = queue(YKRdyList,task);	/* put the task on the queue */
		//	printList(YKRdyList);
		}
//		printString("Next ready task priority: ");
//		printInt(YKRdyList->priority);
//		printNewLine();
		YKExitMutex();
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
	task->next = NULL;
	task->prev = NULL;
	if(queue_head == NULL){
		return task;
	}
	if(queue_head->priority > task->priority){
		task->next = queue_head;
		queue_head->prev = task;
		queue_head = task;
	} else {
		TCBptr temp = queue_head;
		while(temp->next != NULL && (temp->next->priority < task->priority)) {
			temp = temp->next;
		}
		if(temp->next==NULL){
			task->prev = temp;
			temp->next = task;
		}
		else
		{
			task->next = temp->next;
			task->next->prev = task;
			task->prev = temp;
			temp->next = task;
		}
	}
	return queue_head;
}

TCBptr dequeue(TCBptr* queue_head){
	TCBptr next = *queue_head;
	*queue_head = (*queue_head)->next;		//does nothing
	next->next = NULL;
	next->prev = NULL;
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
//	YKEnterMutex();
//	printString("ReadyList: ");
//	printList(YKRdyList);
//	printString("suspend task (current list)\n");
//	printString("Suspended List: ");
//	printList(YKSuspList);
	temp = peak(YKRdyList);
	task = dequeue(&YKRdyList);
	task->state = DELAYED;
//	printString("task prioirty: ");
//	printInt(task->priority);
//	printString(" task delay: ");
//	printInt(task->delay);
//	printNewLine();
//	printString("YKSuspList: ");
//	printInt((int)YKSuspList);
//	printNewLine();
//	printString("Top delay: ");
//	printInt(YKSuspList->delay);
//	printNewLine();
	if(YKSuspList == NULL){
//		printString("first suspended task\n");
		YKSuspList = task;//dequeue(&YKRdyList);
//		printString("Ready: ");
//		printList(YKRdyList);
//		printString("Suspended: ");
//		printList(YKSuspList);
	}
	else if(task->delay <= YKSuspList->delay){
//		printString("task to front of queue\n");
		//task = dequeue(&YKRdyList);
		task->next = YKSuspList;
		task->prev = NULL;
		YKSuspList->prev = task;
		YKSuspList->delay -= task->delay;
                YKSuspList = task;
	} else { /* Needs fixin later*/
		//task->delay -= YKSuspList->delay;
		//temp = YKSuspList->next;
		temp = YKSuspList;
		while(temp->next != NULL && temp->delay < task->delay){
			//printString("decrement delay\n");
			task->delay -= temp->delay;
			temp = temp->next;
		}
//		printString("temp priority: ");
//		printInt(temp->priority);
//		printString(" temp delay: ");
//		printInt(temp->delay);
//		printNewLine();
//		printString("task delay: ");
//		printInt(task->delay);
//		printNewLine();
		if(temp->delay < task->delay) {			/* end of the list */
			task->delay -= temp->delay;
			temp->next = task;
			task->prev = temp;
		} else {					/* insert into list */
			temp->prev->next = task;
			task->prev = temp->prev;
			task->next = temp;
			temp->prev = task;
			temp->delay -= task->delay;
		}
	}
//	printString("Suspended List: ");
//	printList(YKSuspList);
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
//	YKExitMutex();
}

void saveStack(void* sp){
	runningTask->sp = sp;
}

YKSEM* YKSemCreate(int initialValue){
	static int index = 0;
	//create sem at index
	YKEnterMutex();
	SStack[index].ID = index;
	SStack[index].value = initialValue;
	YKExitMutex();
	//return address and increment index
	return &SStack[index++];
}

void YKSemPend(YKSEM* semaphore){
	TCBptr task;
	YKEnterMutex();
//	printString("\nPend:\n");
//	printString("ReadyList: ");
//	printList(YKRdyList);

	if(semaphore->value-- > 0){
		YKExitMutex();
		return;
	}
	task = dequeue(&YKRdyList);
	task->state = SUSPENDED;
	YKPendList[semaphore->ID] = queue(YKPendList[semaphore->ID],task);
//	printString("Semaphore ");
//	printInt(semaphore->ID);
//	printString(" list: ");
//	printList(YKPendList[semaphore->ID]);
//	printString("ReadyList: ");
//	printList(YKRdyList);
	YKScheduler(0);
	YKExitMutex();
}

void YKSemPost(YKSEM* semaphore){
	TCBptr task;
	YKEnterMutex();
//	printString("\nPost:\n");
//	printString("\nSemaphore ");
//	printInt(semaphore->ID);
//	printString("\n");
//	printString(" list: ");
//	printList(YKPendList[semaphore->ID]);

//	printString("Semval: ");
//	printInt(semaphore->value);
//	printString("\n");

	if(semaphore->value++ >= 0){extern TCB    YKTCBArray[MAXTASKS+1];
//		printString("Semval: ");
//		printInt(semaphore->value);
//		printString("\n");
		YKExitMutex();
		return;
	}
//	 else{
//		printString("\nSemaphore ");
//		printInt(semaphore->ID);
//		printString(" list: ");
//	}
	task = dequeue(&YKPendList[semaphore->ID]);
	task->state = READY;
	YKRdyList = queue(YKRdyList,task);
//	printString("Semaphore ");
//	printInt(semaphore->ID);
//	printString(" list: ");
//	printList(YKPendList[semaphore->ID]);
//	printString("ReadyList: ");
//	printList(YKRdyList);

	//call scheduler if not ISR
	if(YKISRDepth == 0){
		YKScheduler(0);
	}
	YKExitMutex();
	
}


YKQ* YKQCreate(void **start, unsigned size){
	static int index = 0;
	YKQ *queue;
	if(index == MAXMESSAGEQUEUES)
		printString("IDIOTS: update max message queues\n");
	//find next entry in QStack
	queue = &QStack[index++];
	//initialize variables
	queue->queue = start;
	queue->head = -1;
	queue->tail = 0;
	queue->length = size;
	return queue;
}

void* YKQPend(YKQ *queue){
	void* msg;
	//check if head == tail
	if(queue->head == -1)
		return NULL;
	msg = queue->queue[queue->head++];
	//update head(increment)
	if(queue->head == queue->length)
		queue->head = 0;
	if(queue->head == queue->tail) // empty
		queue->head = -1;
	return msg;
}

int YKQPost(YKQ *queue, void *msg){
	//check if head == index
	if(queue->head == queue->tail){ // full
		printString("full queue\n");
		printString("head: ");
		printInt(queue->head);
		printString(" tail: ");
		printInt(queue->tail);
		printNewLine();
		return 0;
	}else{ //not full

	//put in msg array
	//update tail
		if(queue->head == -1) // empty
			queue->head = queue->tail;
		queue->queue[queue->tail++] = msg;
		if(queue->tail == queue->length)
			queue->tail = 0;
		return 1;
	}

}

	
