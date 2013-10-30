###########################################################
# Lab 3 ECEn 425 Makefile
# Dayton Minor
# 9/23/2013

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
	rm lab4*_app.i lab4*_app.s lab4*_final.s lab4*.bin lab4*.lst cInt.i cInt.s yakc.i yakc.s
	
