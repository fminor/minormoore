#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"

#define ASTACKSIZE 256          /* Size of each stack in words */
#define BSTACKSIZE 256
#define CSTACKSIZE 256
#define MSGQSIZE   20
#define SEED 1251

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
void postCommand(int cmd, int id, int dir);
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
	SeedSimptris((long) SEED);
	StartSimptris();
	printString("\n\nNew game\n");

	while(1) { /* Pends on command queue */
		YKSemPend(RCSem);
		command = (cmd *) YKQPend(CmdQPtr);
		/* Verify what command and piece */
		printString("piece id: ");
		printInt(command->id);
		printString(" cmd: ");
		printInt(command->cmd);
		printNewLine();
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
	int i, col, id, dest, dir;
	static int next = 0;
	static int next_corner = 0;
	static int space_dir = LEFT;
	static int corner_dir = RIGHT;
	static int corners = 0;
	static int spaces = 0;
	static int flag = 1;
	if(flag) {
		postCommand(SLIDE,5,RIGHT);
		flag = 0;
	}
	while(1){
		/* Removes pieces from piece queue */
		piece = (np *) YKQPend(NPQPtr);
		/* Decides what command to do */
		col = piece->col;
		id = piece->id;
		if(piece->type == STRAIGHT){
			if(col == 5){ /* move the space one left then rotated */
				postCommand(SLIDE, id, LEFT);
				col--;
				postCommand(ROTATE,id,space_dir);
			} else if(col == 0){ /* piece needs to be moved right then rotated */
				postCommand(SLIDE,id,RIGHT);
				col++;
				postCommand(ROTATE,id,space_dir);
			} else if(piece->rot == 1){ /* verticle piece needs to be rotated */
				postCommand(ROTATE,id,RIGHT);
			}
			/* move the piece to it's position */
			if(space_dir == RIGHT){
				dest = 5; // Should be 4
				while(col < dest){
					col++;
					postCommand(SLIDE,id,RIGHT);
				}
			}else{ /* move space to the left section */
				dest = 0; // Should be 1
				while(col > dest){
				//	printString("slide left");
					col--;
					postCommand(SLIDE,id,LEFT);
				}
			}
		} else {
			/*
			 *         X|
			 * 0 -    XX|    rot=1
			 *
			 *       XXO|    rot=3
			 * 1 -   XOO|
			 */
			/* Rotations in the middle */
			if(col > 0 && col < 5){
				switch(piece->rot){
					case 0: // Go from 0 to 1 or 0 to 3
						dir = next_corner==0?LEFT:RIGHT;
						postCommand(ROTATE,id,dir);
						break;
					case 1: 
						if(next_corner){ // Rotate twice to get to 3
							postCommand(ROTATE,id,RIGHT);
							postCommand(ROTATE,id,RIGHT);
						} 
						break;
					case 2: // Go from 2 to 1 or 2 to 3
					        dir = next_corner==0?RIGHT:LEFT;
				       		postCommand(ROTATE,id,dir);
						break;
				 	case 3: 
						if(!next_corner){ // Rotate twice to get to 1
							postCommand(ROTATE,id,RIGHT);
							postCommand(ROTATE,id,RIGHT);
						}
						break;		
				}
			} else if (col ==0) {
				/* 0 can rotate RIGHT
				 * 1 never happens
				 * 2 never happens
				 * 3 can rotate LEFT
				 */
				switch(piece->rot){
					case 0: // Go from 0 to 3, or slide first then rotate
						if(next_corner){ // Slide right first
							postCommand(SLIDE,id,RIGHT);
							col++;
						}
						dir = next_corner==0?LEFT:RIGHT;
						postCommand(ROTATE,id,dir);
						break;
					case 3: // Rotate twice if needed
						if(!next_corner){
							postCommand(ROTATE,id,LEFT);
							postCommand(ROTATE,id,LEFT);
						}
						break;
				}

			} else if (col == 5) {
				/* 1 can rotate LEFT
				 * 2 can rotate RIGHT
				 */
				switch(piece->rot){
					case 1: // Rotate twice to get 3 if needed
					        if(next_corner) {
							postCommand(ROTATE,id,LEFT);
							postCommand(ROTATE,id,LEFT);	
					        }
					        break;
					case 2: // Go from 2 to 1, or rotate first
						if(!next_corner){
							postCommand(SLIDE,id,LEFT);
							col--;
						}
						dir = next_corner==0?RIGHT:LEFT;	
				       		postCommand(ROTATE,id,dir);
						break;
				}
			}
			/* Sliding */
			if (corner_dir = RIGHT){
				dest = next_corner==0?6:4;
				while(col < dest) {
					postCommand(SLIDE,id,RIGHT);
					col++;
				}
			} else {
				dest = next_corner==0?0:3;
				while(col > dest) {
					postCommand(SLIDE,id,LEFT);
					col--;
				}
			}
			next_corner = !next_corner;			
		}
	}
}



void postCommand(int cmd, int id, int dir){
	static int next = 0;
//	printString("Posting command: ");
//	printInt(cmd);
//	printString(", ID: ");
//	printInt(id);
//	printString(", dir: ");
//	printInt(dir);
//	printNewLine();
	CmdArray[next].cmd = cmd;
	CmdArray[next].id = id;
	CmdArray[next].dir = dir;
	/* Put commands on command queue */
	if(YKQPost(CmdQPtr, (void *) &(CmdArray[next])) == 0)
		printString("  Command queue overflow ! \n");
	else if ( ++ next >= CMDARRAYSIZE)
		next = 0;
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
