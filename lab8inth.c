/* File: lab8inth.c
 * Interrupt handlers for simptris lab
 * Dayton Minor
 * Jared Moore
 * 5 Dec 2013
 */
#include "yakk.h"
#include "lab8defs.h"
#include "clib.h"

/* External function prototypes */
extern void gameOver(void);

/* External variables */
extern YKSEM* RCSem;
extern np NPArray[];
extern YKQ* NPQPtr;
extern cmd CmdArray[];
extern YKQ* CmdQPtr;
extern stat StatArray[];
extern YKQ* StatQPtr;

int nextStat = 0;

void handleGO(){ /* Game Over */
	gameOver(); /* In lab8app.c, should list stats */
	exit(0);
}

void handleNP(){ /* New Piece */
	extern int NewPieceID;
	extern int NewPieceType;
	extern int NewPieceOrientation;
	extern int NewPieceColumn;
	static int next = 0;
	NPArray[next].id = NewPieceID;
	NPArray[next].type = NewPieceType;
	NPArray[next].rot = NewPieceOrientation;
	NPArray[next].col = NewPieceColumn;
	/* Puts information on NPQ */
	if(YKQPost(NPQPtr, (void *) &(NPArray[next])) == 0)
		printString("  NewPieceISR: queue overflow ! \n");
	else if (++ next >= NPARRAYSIZE)
		next = 0;
}

void handleRC(){ /* Received Command */
	/* Releases semaphore */
	YKSemPost(RCSem);
}
extern int spaces;
void handleTD(){ /* Touchdown */
	extern int TouchdownID;
	extern int ScreenBitMap0;
	extern int ScreenBitMap1;
	extern int ScreenBitMap2;
	extern int ScreenBitMap3;
	extern int ScreenBitMap4;
	extern int ScreenBitMap5;
	StatArray[nextStat].type = TD;
	StatArray[nextStat].sbm0 = ScreenBitMap0;
	StatArray[nextStat].sbm1 = ScreenBitMap1;
	StatArray[nextStat].sbm2 = ScreenBitMap2;
	StatArray[nextStat].sbm3 = ScreenBitMap3;
	StatArray[nextStat].sbm4 = ScreenBitMap4;
	StatArray[nextStat].sbm5 = ScreenBitMap5;
	/* Puts information on StatQ */
	if(YKQPost(StatQPtr, (void *) &(StatArray[nextStat])) == 0)
		printString("  TouchdownISR: queue overflow ! \n");
	else if (++ nextStat >= STATARRAYSIZE)
		nextStat = 0;

}

void handleLC(){ /* Line Clear */
	/* Puts information on StatQ */
	StatArray[nextStat].type = CLEAR;
	if(spaces > MAXSPACES)
		spaces--;
	else
		spaces-=2;
	/* Puts information on StatQ */
	if(YKQPost(StatQPtr, (void *) &(StatArray[nextStat])) == 0)
		printString("  LineClearISR: queue overflow ! \n");
	else if (++ nextStat >= STATARRAYSIZE)
		nextStat = 0;
}
