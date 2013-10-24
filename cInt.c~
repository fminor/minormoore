#include "clib.h"

void handleReset(){
	exit(0);
}

// Global variable n for tick
int n = 0;

void handleTick(){
	printNewLine();
	printString("Tick ");
	printInt(n);
	printNewLine();
	n++;
}


void handleKeyboard(){
	extern int KeyBuffer;
	int i;
	if(KeyBuffer == 'd'){
		printNewLine();
		printString("DELAY KEY PRESSED");
		for (i = 0; i < 5000; i++){
			// Do nothing
		} 
		printNewLine();
		printString("DELAY COMPLETE");	
	}
	else {
		printNewLine();
		printString("KEYPRESS '");
		printChar(KeyBuffer);
		printString("' IGNORED");
		printNewLine();
	}
}
