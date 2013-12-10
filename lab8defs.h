#define NPARRAYSIZE 20
#define CMDARRAYSIZE 20
#define STATARRAYSIZE 20
#define FINDSIZE	16

#define CORNER 		0
#define STRAIGHT	1
#define MAXSPACES	4
typedef struct np_ {
	int id;
	int type; /* Corner or Straight */
	int rot; /* Orientation */
	int col; /* Center block of piece */
} np;

#define SLIDE		0
#define ROTATE		1

#define LEFT		0
#define RIGHT		1

#define CLK		1
#define CNTRCLK		0
typedef struct cmd_ * cmdPtr;
typedef struct cmd_ {
	int cmd; /* Slide or Rotate */
	int id; /* ID of piece */
	int dir;
	cmdPtr next;
} cmd;

#define CLEAR		0
#define TD		1

typedef struct stat_ {
	int type; /* CLEAR or TD */
	int sbm0; /* 16 bit integers lol */
	int sbm1;
	int sbm2;
	int sbm3;
	int sbm4;
	int sbm5;
} stat;

/* Single Space States are
 * 0 0	0 0 0 	0 0 	X 0 X	0 0 |X or X| 0 0 
 * X 0	X 0 X   0 X     X 0 X	0 0 |X    X| 0 0 
 * 1A	1AB	1B	1C	1D	
 * Priority
 * Low  Low	Low	High	Med
 *			(strait (strait
 *			  only)  only)
 * X = full space
 * 0 = empty space
 * | = wall
 * |X = wall/full	
 */
#define A1	0
#define B1	1
#define C1	3	
#define D1	4

/* Double Space States are
 * 0 0 		0 0 |X or X| 0 0 
 * 1A		2B
 * 		Preferred		
 */
#define A2	5
#define B2	6

/* Triple Space States are
 * 0 0 		0 0 |X or X| 0 0 
 * 3A		3B
 * 		Preferred		
 */
#define A3	7
#define B3	8

typedef struct space_j {
	int state; 	/* State of the space */
	int row; 	/* Row of space */
	int col;	/* Location of space */
	int rot;	/* Same as in new piece */
	int cost;	/* Cost of space */
	cmdPtr commands;
	int priority;	/* Priority */
} space_t;			/* Could include a linked list of cmd */

/* Single space rotation costs
 * Corner Piece States are (for reference):
 * X	 X	XX	XX
 * XX	XX	 X	X
 * 0	1	2	4
 * Transitions are # -> State
 * 0 -> A	RR
 * 1 -> A	L
 * 2 -> A	N
 * 3 -> A	R
 *
 * 0 -> B	R	
 * 1 -> B	RR
 * 2 -> B	L
 * 3 -> B	N
 *
 * R = rotate Right
 * RR = rotate Right twice
 * L = rotate Left
 * N = No action taken
 * If two actions must be taken, it will always be RR
 */

