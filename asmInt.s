; ISR responsible for saving state
; ISR for Reset (Ctrl-R)
ResetISR:
	push ax
	push bx
	push cx 
	push dx
	push si
	push di
	push bp
	push es
	push ds 
	sti
	call handleReset
	cli
	pop ds
	pop es
	pop bp
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	mov al, 0x20	; Load nonspecific EOI value into register al
	out 0x20, al	; Write EOI to PIC (port 0x20)
	pop ax
	iret

; ISR for Tick (Ctrl-T)
TickISR:
	push ax
	push bx
	push cx 
	push dx
	push si
	push di
	push bp
	push es
	push ds

	sti 
	call handleTick
	cli

	pop ds
	pop es
	pop bp
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	mov al, 0x20	; Load nonspecific EOI value into register al
	out 0x20, al	; Write EOI to PIC (port 0x20)
	pop ax
	iret

; ISR for Keyboard Interrupt (Any other key)
KeyboardISR:
	push ax
	push bx
	push cx 
	push dx
	push si
	push di
	push bp
	push es
	push ds
	sti 
	call handleKeyboard
	cli
 
	pop ds
	pop es
	pop bp
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	mov al, 0x20	; Load nonspecific EOI value into register al
	out 0x20, al	; Write EOI to PIC (port 0x20)
	pop ax
	iret
