#include "clib.h"
#include "yakk.h"

void handleReset(){
	exit(0);
}

// Global variable n for tick
extern int YKTickNum;


extern YKSEM *NSemPtr;
void handleKeyboard(){
	extern int KeyBuffer;
	/*int i;
	if(KeyBuffer == 'd'){
		printNewLine();
		printString("DELAY KEY PRESSED");
		for (i = 0; i < 5000; i++){
			// Do nothing
		} 
		printNewLine();
		printString("DELAY COMPLETE");	
	}*/
	if(KeyBuffer == 'p'){
		YKSemPost(NSemPtr);	
	}
	else {
		printNewLine();
		printString("KEYPRESS '");
		printChar(KeyBuffer);
		printString("' IGNORED");
		printNewLine();
	}
//
}
