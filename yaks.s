; Filename: yaks.s 
; Dayton Minor
; Jared Moore
; Lab4a
;


; initStack initialized stack for new tasks 
; param1 cs bp + 2
; param2 ip bp + 4
; param3 ss bp + 6
; param4 sp bp + 8
;
; stack should look like this when we are done:
; ax = 0
; bx = 0
; cx = 0
; dx = 0
; si = 0
; di = 0
; ds = 0
; es = 0
; ------
; bp = param4 = bp + 8
; ip = param2 = bp + 4
; cs = param1 = bp + 2
; flags = 0000 0010 0000 0000 = 0x0200  
; flags are: {15,14,13,12,11(OF),10(DF),9(IF),8(TF),7(SF),6(ZF),5,4(AF),3,2(PF),1,0(CF)}
initStack:
	push bp		;
	mov bp,sp	;
	mov ax, 0x0200	; Store flags in ax
	mov bx, word[bp + 8]	; Store TCB sp in bx
	mov cx, word[bp + 4]	; store ip in cx
	mov dx, word[bp + 2]	; store cs in dx
	mov sp, word[bp + 8]	; Change stack pointer to TCB stack
	mov ss, word[bp + 6]	; 
	push ax		; Push flags
	push dx		; Push cs
	push cx		; Push ip
	push bx		; Push bp
	mov ax, 0x0	; 
	push ax		; Push 0 onto stack 8 times
	push ax		; 2
	push ax		; 3
	push ax		; 4
	push ax		; 5
	push ax		; 6
	push ax		; 7
	push ax		; 8
	mov sp,bp	; Restore stack pointer
	pop bp		;
	ret		; Return
		 
	 
	

