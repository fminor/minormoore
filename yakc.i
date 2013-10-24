# 1 "yakc.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "yakc.c"
# 9 "yakc.c"
# 1 "clib.h" 1



void print(char *string, int length);
void printNewLine(void);
void printChar(char c);
void printString(char *string);


void printInt(int val);
void printLong(long val);
void printUInt(unsigned val);
void printULong(unsigned long val);


void printByte(char val);
void printWord(int val);
void printDWord(long val);


void exit(unsigned char code);


void signalEOI(void);
# 10 "yakc.c" 2
# 1 "yakk.h" 1
# 9 "yakk.h"
# 1 "yaku.h" 1
# 10 "yakk.h" 2
# 20 "yakk.h"
typedef struct taskblock *TCBptr;
typedef struct taskblock {
 void* sp;
 int state;
 unsigned char priority;
 int delay;

 TCBptr next;
 TCBptr prev;
} TCB;

extern TCBptr YKRdyList;

extern TCBptr YKSuspList;
extern TCBptr YKAvailTCBList;
extern TCB YKTCBArray[3 +1];






void YKInitialize();
void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority);
void YKRun();
void YKDelayTask(int count);
void YKEnterISR();
void YKExitISR();
void YKScheduler();
void YKDispatcher(TCBptr);
void YKTickHandler();


void YKEnterMutex();
void YKExitMutex();
void YKIdle();
int initStack(int, int);
TCBptr queue(TCBptr, TCBptr);
TCBptr dequeue(TCBptr);
TCBptr peak(TCBptr);
void suspendTask(TCBptr);
void dispatchTask(void* );



extern int YKCtxSwCount;
extern int YKIdleCount;
extern int YKTickNum;
# 11 "yakc.c" 2


int YKCtxSwCount;
int YKIdleCount;
int YKTickNum;
int YKISRDepth;

int activeTasks;

TCBptr runningTask;
TCBptr YKRdyList;

TCBptr YKSuspList;
TCBptr YKAvailTCBList;
TCB YKTCBArray[3 +1];

int IStk[255];



void YKInitialize() {
 int i;
 TCBptr tempList;
 printString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
 activeTasks = 0;
 YKCtxSwCount = 0;
 YKIdleCount = 0;
 YKTickNum = 0;

 YKRdyList = 0;
 YKSuspList = 0;
 runningTask = 0;
 YKAvailTCBList = YKTCBArray;
 YKTCBArray[0].prev = 0;
 for(i = 0; i < 3 - 1; i++){
  YKTCBArray[i].next = &YKTCBArray[i + 1];
  YKTCBArray[i + 1].prev = &YKTCBArray[i];
 }

 YKNewTask(YKIdle, (void*)&IStk[255],255);

}


void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) {
 int ip,sp;


 TCBptr new_task = &YKTCBArray[activeTasks];

 activeTasks++;
 new_task->priority = priority;
 new_task->state = 0;
 new_task->delay = 0;
 YKRdyList = queue(YKRdyList,new_task);
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







 if(runningTask != 0){
  YKScheduler();
 }

}




void YKRun() {

 YKScheduler();
}

void YKDelayTask(int count) {

 printString("Delayed Task: ");
 printInt(runningTask->priority);
 printNewLine();
 runningTask->delay=count;
 printString("Delay for: ");
 printInt(runningTask->delay);
 printNewLine();
 suspendTask(runningTask);
 printString("Top of the Suspended List: ");
 printInt(YKSuspList->priority);
 printNewLine();
 printString("Delayed for: ");
 printInt(YKSuspList->delay);
 printNewLine();
 YKScheduler();
}
# 133 "yakc.c"
void YKScheduler() {
 TCBptr next;




 next = peak(YKRdyList);
# 163 "yakc.c"
 printString("\nnext task priority: ");
 printInt(next->priority);
 printNewLine();

 if(runningTask != 0 && runningTask->priority != next->priority){
  printString("Context Switch\n");
  YKCtxSwCount++;
 }

 YKDispatcher(next);

}


void YKDispatcher(TCBptr next) {
 void* sp;
 printString("Dispatcher\n\r");
 runningTask = next;
 sp = next->sp;
 printString("Stack: \n");
 printInt((int)sp);
 next->state = 1;

 dispatchTask(sp);

}

void YKTickHandler() {

 printNewLine();
 printString("Tick ");
 printInt(YKTickNum);
 printNewLine();

 YKTickNum++;



 YKSuspList->delay = YKSuspList->delay - 1;
 printString("Top delay: \n");
 printInt(YKSuspList->delay);
 printNewLine();
 while(YKSuspList->delay == 0){
  TCBptr task = dequeue(YKSuspList);
  YKSuspList = task->next;
  task->next = 0;
  YKRdyList = queue(YKRdyList,task);
 }
}

void YKIdle() {
 int i;
 while(1) {
  YKIdleCount++;
  i = 0;
  i = 0;
  for(i = 0; i < 500; i++);
  printString("Idle geese\n");
 }
}
TCBptr peak(TCBptr head){
 return head;
}
TCBptr queue(TCBptr queue_head, TCBptr task){
 if(queue_head == 0){
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







 return next;
}

void suspendTask(TCBptr task){
 TCBptr temp = 0;
 printString("suspend task\n");
 task->state = 2;
 printString("task delay: ");
 printInt(task->delay);
 printNewLine();
 printString("YKSuspList: ");
 printInt((int)YKSuspList);
 printNewLine();
 printString("Top delay: ");
 printNewLine();
 printInt(YKSuspList->delay);
 printNewLine();
 if(YKSuspList == 0){
  printString("first suspended task\n");
  YKSuspList = task;
 }
 else if(task->delay < YKSuspList->delay){
  printString("task to front of queue\n");
  YKSuspList->delay -= task->delay;
                YKSuspList = task;
 } else {


  temp = YKSuspList;
  while(temp->next != 0 && temp->delay < task->delay){
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
 if(YKRdyList != 0 && YKRdyList->priority == task->priority){
  YKRdyList = YKRdyList->next;
 }
}
