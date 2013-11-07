; ISR responsible for saving state
; ISR for Reset (Ctrl-R)
ResetISR:
	push bp
	push es
	push ds
	push di
	push si
	push dx
	push cx 
	push bx
	push ax
	mov si, [runningTask]   ; Save stack pointer
        mov [si], sp            ;
	call YKEnterISR
	sti
	call handleReset
	cli
	mov al, 0x20	; Load nonspecific EOI value into register al
	out 0x20, al	; Write EOI to PIC (port 0x20)
	call YKExitISR
	pop ax			;
	pop bx			;
	pop cx			;
	pop dx			;
	pop si			;
	pop di			;
	pop ds			;
	pop es			;
	pop bp
	iret

; ISR for Tick (Ctrl-T)
TickISR:
	push bp
	push es
	push ds
	push di
	push si
	push dx
	push cx 
	push bx
	push ax
;	mov ax, word[runningTask]
;	mov [ax], sp			;this line is necessary
;	push sp
;	call saveStack
;	pop ax
	mov si, [runningTask]   ; Save stack pointer
        mov [si], sp            ;
	call YKEnterISR
	sti 
	call YKTickHandler
	cli
	mov al, 0x20		; Load nonspecific EOI value into register al
	out 0x20, al		; Write EOI to PIC (port 0x20)
	call YKExitISR

	pop ax			;
	pop bx			;
	pop cx			;
	pop dx			;

	pop si			;
	pop di			;
	pop ds			;
	pop es			;
	pop bp

	iret

; ISR for Keyboard Interrupt (Any other key)
KeyboardISR:
	push bp
	push es
	push ds
	push di
	push si

	push dx
	push cx 
	push bx
	push ax

	mov si, [runningTask]   ; Save stack pointer
        mov [si], sp            ;
	call YKEnterISR
	sti 
	call handleKeyboard
	cli
	mov al, 0x20	; Load nonspecific EOI value into register al
	out 0x20, al	; Write EOI to PIC (port 0x20)
 	call YKExitISR

	pop ax			;
	pop bx			;
	pop cx			;
	pop dx			;

	pop si			;
	pop di			;
	pop ds			;
	pop es			;
	pop bp

	iret
