; Filename: yaks.s 
; Dayton Minor
; Jared Moore
; Lab4c
;


; initStack initialized stack for new tasks 
; param1 ip bp + 4
; param2 sp bp + 6
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
	push bp					;
	mov bp,sp				;
	push bx					;
	push cx					;
	push dx					;
	mov ax, 0x0				; 
	mov bx, word[bp + 6]			; Store TCB sp in bx
	mov cx, word[bp + 4]			; store ip in cx
	mov dx, 0x0200				; Store flags in dx
	mov bp, sp
	mov sp, bx				; Change stack pointer to TCB stack
	push dx					; Push flags
	push ax					; Push cs
	push cx					; Push ip
	push bx					; Push bp
	push ax					; Push 0 onto stack 8 times
	push ax					; 2
	push ax					; 3
	push ax					; 4
	push ax					; 5
	push ax					; 6
	push ax					; 7
	push ax					; 8  Everything is good to this point
	mov ax,sp				; Save new sp
	mov sp,bp				; Restore stack pointer
	pop dx					; Restore bx - dx, bp
	pop cx
	pop bx
	pop bp					;
	ret						; Return
YKSaveContext:
	;ctx ret		; old bp+2 IP
	;pushf
	;push cs
	;push ctx ret
	;push es-ax
	;ret - bp+2 -> bp-18 Goes back to what called this function
	ret

dispatchTask:
	push bp			;
	mov bp,sp		;
	mov sp, word[bp+4]	;
	pop ax			;
	pop bx			;
	pop cx			;
	pop dx			;
	pop si			;
	pop di			;
	pop ds			;
	pop es			;
	pop bp			;
	iret			; Return
		 
YKEnterMutex:
	push bp
	mov bp, sp
	cli
	mov sp, bp
	pop bp
	ret

YKExitMutex:
	push bp
	mov bp, sp
	sti
	mov sp, bp
	pop bp
	ret
	
YKEnterISR:
	inc word[YKISRDepth]
	ret

YKExitISR:
	dec word[YKISRDepth]
	cmp word[YKISRDepth], 0
	je YKScheduler
	ret
