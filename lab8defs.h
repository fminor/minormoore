
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
