# 1 "cInt.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "cInt.c"
# 1 "clib.h" 1



void print(char *string, int length);
void printNewLine(void);
void printChar(char c);
void printString(char *string);


void printInt(int val);
void printLong(long val);
void printUInt(unsigned val);
void printULong(unsigned long val);


void printByte(char val);
void printWord(int val);
void printDWord(long val);


void exit(unsigned char code);


void signalEOI(void);
# 2 "cInt.c" 2

void handleReset(){
 exit(0);
}


extern int YKTickNum;

void handleTick(){
 printNewLine();
 printString("Tick ");
 printInt(YKTickNum);
 printNewLine();
 YKTickNum++;
}


void handleKeyboard(){
 extern int KeyBuffer;
 int i;
 if(KeyBuffer == 'd'){
  printNewLine();
  printString("DELAY KEY PRESSED");
  for (i = 0; i < 5000; i++){

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
