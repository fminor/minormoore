#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"

#define ASTACKSIZE 256          /* Size of each stack in words */
#define BSTACKSIZE 256
#define CSTACKSIZE 256
#define MSGQSIZE   20
#define SEED 1247

np NPArray[NPARRAYSIZE]; 	/* buffers for new piece content */
cmd CmdArray[CMDARRAYSIZE]; 	/* buffers for command content */
stat StatArray[STATARRAYSIZE];   /* buffers for stat content */

int AStk[ASTACKSIZE];           /* Space for each task's stack */
int BStk[BSTACKSIZE];
int CStk[CSTACKSIZE];

void *NPQ[MSGQSIZE];		/* space for message queues */
void *CmdQ[MSGQSIZE];
void *StatQ[MSGQSIZE];

YKQ* NPQPtr;			/* actual names of queues */
YKQ* CmdQPtr;
YKQ* StatQPtr;

void ATask(void);               /* Function prototypes for task code */
void BTask(void);
void CTask(void);

YKSEM* RCSem; /* Semaphore for received command */

/* Function prototypes for utility code */
int find3space(int col); /* Finds nearest space 3 wide */
int find2space(int col);
int find1space(int col);
void gameOver(void); /* Lists end of game stats ?*/

/* Global variables */
int lines = 0;	/* total number of lines cleared */
int pieces = 0; /* number of pieces received */

void main(void) {
	YKInitialize();

	YKNewTask(ATask, (void *)&AStk[ASTACKSIZE], 5);
	YKNewTask(BTask, (void *)&BStk[ASTACKSIZE], 15);
	YKNewTask(CTask, (void *)&CStk[ASTACKSIZE], 25);

	RCSem = YKSemCreate(1);
	NPQPtr = YKQCreate(NPQ, MSGQSIZE);
	CmdQPtr = YKQCreate(CmdQ, MSGQSIZE);
	StatQPtr = YKQCreate(StatQ, MSGQSIZE);

	YKRun();
}

void ATask(void){ /* Communicates with simptris */
	cmd* command;
	SeedSimptris(SEED);
	StartSimptris();
	while(1) { /* Pends on command queue */
		YKSemPend(RCSem);
		command = (cmd *) YKQPend(CmdQPtr);
		/* Does something with command */
		if (command->cmd == SLIDE) {
			SlidePiece(command->id, command->dir);
		} else {
			RotatePiece(command->id, command->dir);
		}
	}	
}

void BTask(void){ /* New Piece */
	np* piece;
	int i, slides;
	static next;
	while(1){
		/* Removes pieces from piece queue */
		piece = (np *) YKQPend(NPQPtr);
		/* Decides what command to do */
		slides = piece->col;
		for(i = 0; i < slides; i++){ 
			CmdArray[next].id = piece->id;
			CmdArray[next].cmd = SLIDE;
			CmdArray[next].dir = LEFT;
		/* Put commands on command queue */
			if(YKQPost(CmdQPtr, (void *) &(CmdArray[next])) == 0)
				printString("  Command queue overflow ! \n");
			else if ( ++ next >= CMDARRAYSIZE)
				next = 0;
		}

	}
}

void CTask(void){ /* Statistics */
	stat* stats;
	while(1){
		/* Gets stats from stats queue */
		stats = (struct stat *) YKQPend(StatQPtr);
		if (stats->type == CLEAR) 
			lines++;
		
	}

}

void gameOver(){

}
