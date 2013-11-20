; ISR responsible for saving state
; ISR for Reset (Ctrl-R)
ResetISR:
	call YKEnterISR
	sti
	call handleReset
	cli
	jmp YKExitISR

; ISR for Tick (Ctrl-T)
TickISR:
	call YKEnterISR
	sti 
	call YKTickHandler
;	call mytick
	cli
	jmp YKExitISR

; ISR for Keyboard Interrupt (Any other key)
KeyboardISR:
	call YKEnterISR
	sti 
	call handleKeyboard
;	call mykeybrd
	cli
 	jmp YKExitISR
