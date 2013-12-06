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
	cli
	jmp YKExitISR

; ISR for Keyboard Interrupt (Any other key)
KeyboardISR:
	call YKEnterISR
	sti 
	call handleKeyboard
	cli
 	jmp YKExitISR

;ISR for Game Over Interrupt
GameOverISR:
	call YKEnterISR
	sti
	call handleGO
	cli
	jmp YKExitISR

;ISR for New Piece Interrupt
NewPieceISR:
	call YKEnterISR
	sti
	call handleNP
	cli
	jmp YKExitISR

;ISR for Received Command Interrupt
ReceivedISR:
	call YKEnterISR
	sti
	call handleRC
	cli
	jmp YKExitISR


;ISR for Touchdown Interrupt
TouchdownISR:
	call YKEnterISR
	sti
	call handleTD
	cli
	jmp YKExitISR

;ISR for Line Clear Interrupt
LineClearISR:
	call YKEnterISR
	sti
	call handleLC
	cli
	jmp YKExitISR



