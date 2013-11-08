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



;YKSaveContext saves context in case we switch tasks
;ctx ret to task         <-old word[word[bp] + 2]
;ctx bp                  <-old bp = word[bp]
;caller local variables
;ret add to delay task   <- 24 
;cs                      <- 22
;ip                      <- 20
;ctx bp                  <- 18
;es                      <- 16
;ds                      <- 14
;di                      <- 12
;si                      <- 10
;dx                      <- 8
;cx                      <- 6 
;bx                      <- sp + 4
;ax                      <- sp + 2(Save sp here)
;ret add to dispatcher   <- sp
;(Change bp to sp, so we don't overwrite everything we just did)
YKSaveContext:
	pushf 			; push flags
	push cs 		; push cs 
	push word[bp + 2]	; ctx ret
	push word[bp]		; push old bp, this will clean up the stack when we return to the task

	push es			; push es,ds,di,si,dx,cx,bx,ax
	push ds			;
	push di			;
	push si			;
	push dx			;
	push cx			;
	push bx			;
	push ax			;
	mov si, [runningTask]	; Save stack pointer
	mov [si], sp		;
;	mov si, word[bp]		; 
;	add si, 2		;
;	add sp, 20		;
;	push word[si]		;
;	sub sp, 18		;
labelSave:
				; Leave stack
	push word[bp - 4]	; Push ret address
	ret

dispatchTask:
	push bp			;
	mov bp,sp		;
	mov sp, word[bp+4]	;
isrRestore:
	pop ax			;
	pop bx			;
	pop cx			;
	pop dx			;

	pop si			;
	pop di			;
	pop ds			;
	pop es			;

	pop bp			;

labelDispatch:
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
;	flags already pushed
;	cs already pushed
;	iret address
;	push bp ; 16 (ret address is here)
	push es ; 14 
	push ds ; 12
	push di ; 10
	push si ; 8
	push dx ; 6
	push cx ; 4
	push bx ; 2
	push ax ; 0
	mov si, [runningTask]   ; Save stack pointer
        mov [si], sp            ;
	mov ax, bp
	mov bp, sp		; 
	push word[bp + 16]	; Return address to ISR
	mov [bp + 16], ax 	; Saves bp in right place
	inc word[YKISRDepth]
	ret

YKExitISR:
	mov al, 0x20	; Load nonspecific EOI value into register al
	out 0x20, al	; Write EOI to PIC (port 0x20)
	dec word[YKISRDepth]
	cmp word[YKISRDepth], 0
	je endISR
	jmp isrRestore
endISR:
	mov ax, 1
	push ax
	call YKScheduler
	pop ax
	jmp isrRestore
	
