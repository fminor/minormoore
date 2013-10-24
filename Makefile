###########################################################
# Lab 3 ECEn 425 Makefile
# Dayton Minor
# 9/23/2013

all:
	cpp -I ../ lab4c_app.c lab4c_app.i
	cpp -I ../ cInt.c cInt.i
	cpp -I ../ yakc.c yakc.i
	c86 -g lab4c_app.i lab4c_app.s
	c86 -g cInt.i cInt.s
	c86 -g yakc.i yakc.s
	cat clib.s asmInt.s cInt.s yakc.s yaks.s lab4c_app.s > lab4c_final.s
	nasm lab4c_final.s -o lab4c.bin -l lab4c.lst

clean:
	rm lab4c_app.i lab4c_app.s lab4c_final.s lab4c.bin lab4c.lst Emu86OutputPipe_fminor cInt.i cInt.s yakc.i yakc.s
	
