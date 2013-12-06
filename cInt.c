#include "clib.h"
#include "yakk.h"

void handleReset(){
	exit(0);
}

// Global variable n for tick
extern int YKTickNum;

void handleKeyboard(){
	char c;
	extern int KeyBuffer;
   	c = KeyBuffer;

    	    print("\nKEYPRESS (", 11);
    	    printChar(c);
    	    print(") IGNORED\n", 10);
}
