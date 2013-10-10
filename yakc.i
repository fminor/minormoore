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
void suspendTask(TCBptr);
void dispatchTask(void* );



extern int YKCtxSwCount;
extern int YKIdleCount;
extern int YKTickNum;
# 11 "yakc.c" 2


int YKCtxSwCount;
int YKIdleCount;
int YKTickNum;

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
 activeTasks = 0;
 YKCtxSwCount = 0;
 YKIdleCount = 0;
 YKTickNum = 0;

 YKRdyList = 0;
 YKSuspList = 0;
 YKAvailTCBList = YKTCBArray;
 YKTCBArray[0].prev = 0;
 for(i = 0; i < 3 + 1; i++){
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
 sp = initStack(ip,sp);
 new_task->sp = (void*)sp;







 if(runningTask != 0){
  YKScheduler();
 }

}




void YKRun() {

 YKScheduler();
}

void YKDelayTask(int count) {

 runningTask->delay=count;
 suspendTask(runningTask);
}
# 107 "yakc.c"
void YKEnterISR() {

}

void YKExitISR() {

}


void YKScheduler() {
 TCBptr next;




 next = dequeue(YKRdyList);
# 141 "yakc.c"
 if((runningTask != 0) && (next->priority > runningTask->priority)){

  YKRdyList = queue(YKRdyList,next);
 }else {

  YKCtxSwCount++;
  YKRdyList = queue(YKRdyList,runningTask);
  YKDispatcher(next);
 }
}


void YKDispatcher(TCBptr next) {
 void* sp;

 runningTask = next;
 sp = next->sp;
 next->state = 1;
 YKEnterMutex();
 dispatchTask(sp);
 YKExitMutex();
}

void YKTickHandler() {

 YKSuspList->delay = YKSuspList->delay - 1;
 if(YKSuspList->delay == 0){
  TCBptr task = dequeue(YKSuspList);
  YKRdyList = queue(YKRdyList,task);
 }
}

void YKIdle() {
 int i;
 while(1) {
  YKIdleCount++;
  i = 0;
  i = 0;



 }
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
 queue_head = queue_head->next;






 return next;
}

void suspendTask(TCBptr task){
 TCBptr temp = 0;
 task->state = 2;
 if(task->delay < YKSuspList->delay){
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
}
