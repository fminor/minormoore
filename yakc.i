# 1 "yakc.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "yakc.c"
# 9 "yakc.c"
# 1 "../clib.h" 1



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
void YKEnterMutex();
void YKExitMutex();
void YKEnterISR();
void YKExitISR();
void YKScheduler();
void YKDispatcher();
void YKTickHandler();



extern int YKCtxSwCount;
extern int YKIdleCount;
extern int YKTickNum;
# 11 "yakc.c" 2


int YKCtxSwCount;
int YKIdleCount;
int YKTickNum;

TCBptr YKRdyList;

TCBptr YKSuspList;
TCBptr YKAvailTCBList;
TCB YKTCBArray[3 +1];




void YKInitialize() {
 YKCtxSwCount = 0;
 YKIdleCount = 0;
 YKTickNum = 0;







}

void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) {

}

void YKRun() {

}

void YKDelayTask(int count) {

}

void YKEnterMutex() {

}

void YKExitMutex() {

}

void YKEnterISR() {

}

void YKExitISR() {

}

void YKScheduler() {

}

void YKDispatcher() {

}

void YKTickHandler() {

}
