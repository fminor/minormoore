
#define NPARRAYSIZE 20
#define CMDARRAYSIZE 20
#define STATARRAYSIZE 20

#define CORNER 		0
#define STRAIGHT	1

struct np {
	int id;
	int type; /* Corner or Straight */
	int rot; /* Orientation */
	int col; /* Center block of piece */
};

#define SLIDE		0
#define ROTATE		1

#define LEFT		0
#define RIGHT		1

#define CLK		1
#define CNTRCLK		0

struct cmd {
	int cmd; /* Slide or Rotate */
	int id; /* ID of piece */
	int dir;
};

#define CLEAR		0
#define TD		1

struct stat {
	int type; /* CLEAR or TD */
	int sbm0; /* 16 bit integers lol */
	int sbm1;
	int sbm2;
	int sbm3;
	int sbm4;
	int sbm5;
};

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
#define 1A 	0
#define 1B	1
#define 1AB	2
#define 1C	3	
#define 1D	4

/* Double Space States are
 * 0 0 		0 0 |X or X| 0 0 
 * 1A		2B
 * 		Preferred		
 */
#define 2A	5
#define 2B	6

/* Triple Space States are
 * 0 0 		0 0 |X or X| 0 0 
 * 3A		3B
 * 		Preferred		
 */
#define 3A	7
#define 3B	8

struct space_t {
	int state; 	/* State of the space */
	int col;	/* Location of space */
	int cost;	/* Cost of space */
	int priority;	/* Priority */
};			/* Could include a linked list of cmd */

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

