; ISR responsible for saving state
; ISR for Reset (Ctrl-R)
ResetISR:
	push bp
	mov bp,sp
	call YKEnterISR
	sti
	call handleReset
	cli
	jmp YKExitISR

; ISR for Tick (Ctrl-T)
TickISR:
;	push bp
;	push es
;	push ds
;	push di
;	push si
;	push dx
;	push cx 
;	push bx
;	push ax
;	mov ax, word[runningTask]
;	mov [ax], sp			;this line is necessary
;	mov si, [runningTask]   ; Save stack pointer
;       mov [si], sp            ;
	push bp
	mov bp,sp
	call YKEnterISR
	sti 
	call YKTickHandler
	cli
	jmp YKExitISR

;	pop ax			;
;	pop bx			;
;	pop cx			;
;	pop dx			;

;	pop si			;
;	pop di			;
;	pop ds			;
;	pop es			;
;	pop bp

;	iret

; ISR for Keyboard Interrupt (Any other key)
KeyboardISR:
	push bp
	mov bp,sp
	call YKEnterISR
	sti 
	call handleKeyboard
	cli
 	jmp YKExitISR
