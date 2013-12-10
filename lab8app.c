#include "clib.h"
#include "yakk.h"
#include "lab8defs.h"
#include "simptris.h"
#include "macros.h"

#define ASTACKSIZE 256          /* Size of each stack in words */
#define BSTACKSIZE 256
#define CSTACKSIZE 256
#define MSGQSIZE   20
#define SEED 1247

struct np NPArray[NPARRAYSIZE]; 	/* buffers for new piece content */
struct cmd CmdArray[CMDARRAYSIZE]; 	/* buffers for command content */
struct stat StatArray[STATARRAYSIZE];   /* buffers for stat content */

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
space_t find3space(np* piece); /* Finds nearest space 3 wide */
space_t find2space(np* piece);
space_t find1space(np* piece);
space_t getBestSpace(np* piece);

void updateMap(int, space_t*);
int baseRow = 0;
int map[6][16];

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
	struct cmd *command;
	SeedSimptris(SEED);
	StartSimptris();
	while(1) { 
		YKSemPend(RCSem);  /* Simptris ready for command? */
		command = (struct cmd *) YKQPend(CmdQPtr);/* Pends on command queue */
		/* Does something with command */
		if (command->cmd == SLIDE) {
			SlidePiece(command->id, command->dir);
		} else {
			RotatePiece(command->id, command->dir);
		}
	}	
}

void BTask(void){ /* New Piece */
	struct np * piece;
	cmdPtr command;
	space_t space;
	static next;
	while(1){
		/* Removes pieces from piece queue */
		piece = (struct np *) YKQPend(NPQPtr);
		/* Decides what command to do */
		space = getBestSpace(piece);
		/* update map */
		updateMap(piece->type, &space);
		/* Put commands on command queue */
		command = space.commands;
		for( i = 0; i < space->cost; i++){
			CmdArray[next] = (*command);
			command = command->next;
			if(YKQPost(CmdQPtr, (void *) &(CmdArray[next])) == 0)
				printString("  Command queue overflow ! \n");
			else if ( ++ next >= CMDARRAYSIZE)
				next = 0;
		}
	}
}

void CTask(void){ /* Statistics */
	struct stat * stats;
	while(1){
		/* Gets stats from stats queue */
		stats = (struct stat *) YKQPend(StatQPtr);
		if (stats->type == CLEAR) 
			lines++;
		
	}

}

void gameOver(){

}

/**
 * Fills in the spaces where the peices fall
 * and updates baseRow
 *
 */
void updateMap(int type, space_t* space){
	int i,j, lowIndex;
	if(type == STRAIGHT) {
		if(space_t->rot == 0){ /* flat */
			map[space->col - 1][space->row] 	= 1;
			map[space->col][space->row] 		= 1;
			map[space->col + 1][space->row] 	= 1;
		} else {
			map[space->col][space->row - 1] 	= 1;
			map[space->col][space->row] 		= 1;
			map[space->col][space->row + 1] 	= 1;
		}
	} else {
		switch(space_t->rot){
			/* X
			 * XX
			 */
			case 0:
			map[space->col][space->row+1] 		= 1;
			map[space->col][space->row] 		= 1;
			map[space->col+1][space->row] 		= 1;
				       break;	
			/*  X
			 * XX
			 */
			case 1:
			map[space->col-1][space->row] 		= 1;
			map[space->col][space->row] 		= 1;
			map[space->col][space->row+1] 		= 1;
				       break;
			/* XX
			 *  X
			 */
			case 2:
			map[space->col-1][space->row] 		= 1;
			map[space->col][space->row] 		= 1;
			map[space->col][space->row-1] 		= 1;
				       break;
			/* XX
			 * X
			 */
			case 3:
			map[space->col+1][space->row] 		= 1;
			map[space->col][space->row] 		= 1;
			map[space->col][space->row-1] 		= 1;
				       break;
		}
	}
	for(i = 0; i < 6; i++){
		for(j = 15; j >= 0; j--){
			if(map[i][j])
				break;
		}
		if( lowIndex > (j+1))
			lowIndex = j+1;

	}
	baseRow = lowIndex;
}

/**
 * takes a piece as a parameter and returns the highest priority space
 * priority is based off of type of space in relation to type of piece and cost
 * cost: rotations + distance
 *
 */
space_t find1space(np* piece){
	int row, col;
	int space_type;
	space_t bestSpace, curSpace;
	int startBest, startCur;
	/* Array for commands, static so we can make a linked list out of them =) */
	static cmd commands[FINDSIZE];
	startBest = 0;
	startCur = 8;
	//for loop across row starting at base
	for(row = baseRow; row < 16; row++){
	//for loop across col
		for(col = 0; col < 6; col++){
			space_type = 0;
			if(map[col][row] == 0 && map[col][row+1] == 0){
				space_type = space_type | 1;
				//check left
				if(col-1 >= 0 && map[col-1][row] == 1){
					//check up-left
					if(row+1 < 16 && map[col+1][row+1] == 1){ /* C or D*/
					       	//AND NOT L
						/* Set flag to see if it is C or D */
						/* Make D piece */
						updateCost(curSpace,piece,&commands,startCur);

					} else { /* A */
						// Make A space
						updateCost(curSpace,piece,&commands,startCur);
						if(curSpace->cost < bestSpace->cost){
							bestSpace = curSpace;
							startBest = (startBest+8)%16;
							startCur = (startCur+8)%16;
						}
					}
				}
				//check right
				if(col+1 < 6 && map[col+1][row] == 1){
					//check up-right
					if(row+1 < 16 && map[col+1][row+1] == 1){ /* C or D */ 
						//AND NOT L
						if(flag) { /* Make C piece */
							
						} else {
							/* Make D piece */
						}


					} else { /* B */
						/* Make B piece */
						updateCost(curSpace,piece,&commands);
						if(curSpace->cost < bestSpace->cost,startCur){
							bestSpace = curSpace;
							startBest = (startBest+8)%16;
							startCur = (startCur+8)%16;
						}
					}
				}
			}	

		}
	}
}

void updateCost(space_t space,np* piece,cmd* commands, int first){
	int col,col_des,cost;
	int next = first;
	col = piece->col;
	col_des = space->col;
	cost = 0;
	if(col == 0 || col == 5){
		/* handle left right movement */
		if(col < col_des){
			cost++;
			col--;
			commands[next].cmd = SLIDE;
			commands[next].dir = RIGHT;
			commands[next].id = piece->id;
			commands[next].next = &commands[next+1];
			next++;
		}
		if(col > col_des){
			cost++;
			col++;
			commands[next].cmd = SLIDE;
			commands[next].dir = LEFT;
			commands[next].id = piece->id;
			commands[next].next = &commands[next+1];
			next++;
		}
	}
	/* handle rotation movement */
/*	if(space->state == 1A){
		if(piece->type == CORNER){
			
		}else{
			
		}
	}*/
	/* handle left right movement */
	while(col < col_des){
		cost++;
		col--;
		commands[next].cmd = SLIDE;
		commands[next].dir = RIGHT;
		commands[next].id = piece->id;
		commands[next].next = &commands[next+1];
		next++;
	}
	while(col > col_des){
		cost++;
		col++;
		commands[next].cmd = SLIDE;
		commands[next].dir = LEFT;
		commands[next].id = piece->id;
		commands[next].next = &commands[next+1];
		next++;
	}
	space->cost = cost;
	space->commands = &commands[first];
}
