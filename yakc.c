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
YKSEM SStack[MAXSEMAPHORES + MAXMESSAGEQUEUES]; /* Semaphores */
TCBptr YKPendList[MAXSEMAPHORES + MAXMESSAGEQUEUES + MAXEVENTS]; /* One needed for each semaphore */ 

YKQ QStack[MAXMESSAGEQUEUES]; /* Queues */

YKEVENT EStack[MAXEVENTS]; /* Events */ 

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

}

void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) { /* Creates a new task */
	int ip,sp;

	TCBptr new_task = &YKTCBArray[activeTasks];

	activeTasks++;
	new_task->priority = priority;
	new_task->state = READY;
	new_task->delay = 0;
	new_task->next = NULL;
	new_task->prev = NULL;
	YKRdyList = queue(YKRdyList,new_task);
	ip = (int) task & 0xFFFF;
	sp = (int) taskStack & 0xFFFF;

	sp = initStack(ip,sp);
	new_task->sp = (void*)sp;

	if(runningTask != NULL){
		YKScheduler(0);
	}

}



//3
void YKRun() { /* Starts actual execution of user code */

	started = 1;
	YKScheduler(1);
}

void YKDelayTask(int count) { /* Delays a task for specified number of clock ticks*/
	YKEnterMutex();
	runningTask->delay=count;

	suspendTask(runningTask);

	YKExitMutex();
	YKScheduler(0);
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
//	while(YKTickNum > 30);

	//decrement top of YKSuspList->delay
	if(YKSuspList != NULL) {
		YKEnterMutex();
		YKSuspList->delay = YKSuspList->delay - 1;
		while(YKSuspList!= NULL && YKSuspList->delay == 0){
			TCBptr task = dequeue(&YKSuspList);	/* get the top of the list */	
			YKRdyList = queue(YKRdyList,task);	/* put the task on the queue */
		}
		YKExitMutex();
	}
}

void YKIdle() { /* Idle task for the kernel */
	int i;
	while(1) {
		YKIdleCount++;
		i = 0;
		i = 0;
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
	return next;
}

void suspendTask(TCBptr task){
	TCBptr temp = NULL;
	temp = peak(YKRdyList);
	task = dequeue(&YKRdyList);
	task->state = DELAYED;
	if(YKSuspList == NULL){
		YKSuspList = task;//dequeue(&YKRdyList);
	}
	else if(task->delay <= YKSuspList->delay){
		task->next = YKSuspList;
		task->prev = NULL;
		YKSuspList->prev = task;
		YKSuspList->delay -= task->delay;
                YKSuspList = task;
	} else { /* Needs fixin later*/
		temp = YKSuspList;
		while(temp->next != NULL && temp->delay < task->delay){
			//printString("decrement delay\n");
			task->delay -= temp->delay;
			temp = temp->next;
		}
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
	if(semaphore->value-- > 0){
		YKExitMutex();
		return;
	}
	task = dequeue(&YKRdyList);
	task->state = SUSPENDED;
	YKPendList[semaphore->ID] = queue(YKPendList[semaphore->ID],task);

	YKScheduler(0);
	YKExitMutex();
}

void YKSemPost(YKSEM* semaphore){
	TCBptr task;
	YKEnterMutex();

	if(semaphore->value++ >= 0){extern TCB    YKTCBArray[MAXTASKS+1];

		YKExitMutex();
		return;
	}

	task = dequeue(&YKPendList[semaphore->ID]);
	task->state = READY;
	YKRdyList = queue(YKRdyList,task);

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
	queue->sem = YKSemCreate(0);
	return queue;
}

void* YKQPend(YKQ *queue){
	void* msg;
	//check if head == tail
	YKSemPend(queue->sem);
	if(queue->head == -1){
		printString("null value returned");
		return NULL;
	}
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
		//YKSemPost(queue->sem);
		return 0;
	}else{ //not full
		if(queue->head == -1) // empty
			queue->head = queue->tail;
		queue->queue[queue->tail++] = msg;
		if(queue->tail == queue->length)
			queue->tail = 0;
		YKSemPost(queue->sem);
		return 1;
	}

}

YKEVENT* YKEventCreate(unsigned init){
	static int index = 0;
	YKEVENT* e = &EStack[index++];
	e->flags = init;
	e->sFlags = NULL;
	e->tasks = NULL;
	return e;	
}
void eventBlock(YKEVENT* e){
	TCBptr task, list;
	task = dequeue(&YKRdyList);
	task->state = SUSPENDED;
	list = e->tasks;
	if(list == NULL)
		e->tasks = task;
	else{
		task->next = list;
		e->tasks = task;
	}
	YKScheduler(0);
}

unsigned YKEventPend(YKEVENT* e, unsigned mask, int mode){
	unsigned test;
	YKEnterMutex();
	if(mode == EVENT_WAIT_ANY){
		test = mask & e->flags;
		while(!test) {
			YKExitMutex();
			eventBlock(e); /* Block */
			test = mask & e->sFlags; /* test again */
		}
	} else if (mode == EVENT_WAIT_ALL) {
		test = mask & e->flags;
		while(test != mask) {
			YKExitMutex();
			eventBlock(e);
			test = mask & e->sFlags;
		}
	} else {
		printString("Invalid event wait mode!\n");
		return NULL;
	}
	YKExitMutex();
	return e->flags;
}

void YKEventSet(YKEVENT* e, unsigned mask){
	TCBptr head;
	TCBptr temp;
	int schedule;
	YKEnterMutex();
	e->flags |= mask; /* Set bits from mask to one (leave others unchanged) */
	head = e->tasks;
	e->sFlags = e->flags; /* Save flags that caused this pend */
	/* unblock all tasks associated with this event*/
	schedule = 0;
	while( head != NULL){
		temp = head;
		head = head->next;
		e->tasks = head;
		temp->next = NULL;
		YKRdyList = queue(YKRdyList,temp);
		schedule = 1;
	}
	YKExitMutex();
	if(schedule)
		YKScheduler(0);

}

void YKEventReset(YKEVENT* e, unsigned mask){
	e->flags = e->flags & !mask; /* Set the bits from mask to zero in flags */	
}
