###########################################################
# Lab 3 ECEn 425 Makefile
# Dayton Minor
# 9/23/2013

all:
	cpp -I ../ lab4b_app.c lab4b_app.i
	cpp -I ../ cInt.c cInt.i
	cpp -I ../ yakc.c yakc.i
	c86 -g lab4b_app.i lab4b_app.s
	c86 -g cInt.i cInt.s
	c86 -g yakc.i yakc.s
	cat clib.s asmInt.s cInt.s yakc.s yaks.s lab4b_app.s > lab4b_final.s
	nasm lab4b_final.s -o lab4b.bin -l lab4b.lst

clean:
	rm lab4b_app.i lab4b_app.s lab4b_final.s lab4b.bin lab4b.lst Emu86OutputPipe_fminor cInt.i cInt.s yakc.i yakc.s
	
