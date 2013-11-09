###########################################################
# Lab 3 ECEn 425 Makefile
# Dayton Minor
# 9/23/2013
lab6:
	cpp -I ../ lab6app.c lab6_app.i
	cpp -I ../ cInt.c cInt.i
	cpp -I ../ yakc.c yakc.i
	cpp -I ../ lab6inth.c lab6inth.i
	c86 -g lab6_app.i lab6_app.s
	c86 -g lab6inth.i lab6inth.s
	c86 -g cInt.i cInt.s
	c86 -g yakc.i yakc.s
	cat clib.s asmInt.s cInt.s yakc.s yaks.s lab6_app.s lab6inth.s > lab6_final.s
	nasm lab6_final.s -o lab6.bin -l lab6.lst

lab5:
	cpp -I ../ lab5app.c lab5_app.i
	cpp -I ../ cInt.c cInt.i
	cpp -I ../ yakc.c yakc.i
	c86 -g lab5_app.i lab5_app.s
	c86 -g cInt.i cInt.s
	c86 -g yakc.i yakc.s
	cat clib.s asmInt.s cInt.s yakc.s yaks.s lab5_app.s > lab5_final.s
	nasm lab5_final.s -o lab5.bin -l lab5.lst

lab4d:
	cpp -I ../ lab4d_app.c lab4d_app.i
	cpp -I ../ cInt.c cInt.i
	cpp -I ../ yakc.c yakc.i
	c86 -g lab4d_app.i lab4d_app.s
	c86 -g cInt.i cInt.s
	c86 -g yakc.i yakc.s
	cat clib.s asmInt.s cInt.s yakc.s yaks.s lab4d_app.s > lab4d_final.s
	nasm lab4d_final.s -o lab4d.bin -l lab4d.lst

lab4c:
	cpp -I ../ lab4c_app.c lab4c_app.i
	cpp -I ../ cInt.c cInt.i
	cpp -I ../ yakc.c yakc.i
	c86 -g lab4c_app.i lab4c_app.s
	c86 -g cInt.i cInt.s
	c86 -g yakc.i yakc.s
	cat clib.s asmInt.s cInt.s yakc.s yaks.s lab4c_app.s > lab4c_final.s
	nasm lab4c_final.s -o lab4c.bin -l lab4c.lst

lab4b:
	cpp -I ../ lab4b_app.c lab4b_app.i
	cpp -I ../ cInt.c cInt.i
	cpp -I ../ yakc.c yakc.i
	c86 -g lab4b_app.i lab4b_app.s
	c86 -g cInt.i cInt.s
	c86 -g yakc.i yakc.s
	cat clib.s asmInt.s cInt.s yakc.s yaks.s lab4b_app.s > lab4b_final.s
	nasm lab4b_final.s -o lab4b.bin -l lab4b.lst



clean:
	rm lab*_app.i lab*_app.s lab*_final.s lab*.bin lab*.lst cInt.i cInt.s yakc.i yakc.s
	
