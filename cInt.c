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
	extern int YKISRDepth;
	int i;
	if(KeyBuffer == 'd'){
		printNewLine();
		printString("DELAY KEY PRESSED");
		i = 0;
		while(i < 5000)
			i++;	
		printNewLine();
		printString("DELAY COMPLETE");	
	} 
	/*else if(KeyBuffer == 'p'){
		
		printString(" Posting NSemPtr, now: ");
		YKSemPost(NSemPtr);	
		printInt(NSemPtr->value);
		printString(" ISRDepth is: ");
		printInt(YKISRDepth);
	}*/
	else {
		printNewLine();
		printString("KEYPRESS '");
		printChar(KeyBuffer);
		printString("' IGNORED");
		printNewLine();
	}
	printNewLine();
//
}
