; Generated by c86 (BYU-NASM) 5.1 (beta) from lab4b_app.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
L_lab4b_app_2:
	DB	"Starting kernel...",0xA,0
L_lab4b_app_1:
	DB	"Creating task A...",0xA,0
	ALIGN	2
main:
	; >>>>> Line:	23
	; >>>>> { 
	jmp	L_lab4b_app_3
L_lab4b_app_4:
	; >>>>> Line:	24
	; >>>>> YKInitialize(); 
	call	YKInitialize
	; >>>>> Line:	26
	; >>>>> printString("Creating task A...\n"); 
	mov	ax, L_lab4b_app_1
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	27
	; >>>>> YKNewTask(ATask, (void *)&AStk[256], 5); 
	mov	al, 5
	push	ax
	mov	ax, (AStk+512)
	push	ax
	mov	ax, ATask
	push	ax
	call	YKNewTask
	add	sp, 6
	; >>>>> Line:	29
	; >>>>> printString("Starting kernel...\n"); 
	mov	ax, L_lab4b_app_2
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	30
	; >>>>> YKRun(); 
	call	YKRun
	mov	sp, bp
	pop	bp
	ret
L_lab4b_app_3:
	push	bp
	mov	bp, sp
	jmp	L_lab4b_app_4
L_lab4b_app_9:
	DB	"Task A is still running! Oh no! Task A was supposed to stop.",0xA,0
L_lab4b_app_8:
	DB	"Creating task C...",0xA,0
L_lab4b_app_7:
	DB	"Creating low priority task B...",0xA,0
L_lab4b_app_6:
	DB	"Task A started!",0xA,0
	ALIGN	2
ATask:
	; >>>>> Line:	34
	; >>>>> { 
	jmp	L_lab4b_app_10
L_lab4b_app_11:
	; >>>>> Line:	35
	; >>>>> printString("Task 
	mov	ax, L_lab4b_app_6
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	37
	; >>>>> printString("Creating low priority task B...\n"); 
	mov	ax, L_lab4b_app_7
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	38
	; >>>>> YKNewTask(BTask, (void *)&BStk[256], 7); 
	mov	al, 7
	push	ax
	mov	ax, (BStk+512)
	push	ax
	mov	ax, BTask
	push	ax
	call	YKNewTask
	add	sp, 6
	; >>>>> Line:	40
	; >>>>> printString("Creating task C...\n"); 
	mov	ax, L_lab4b_app_8
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	41
	; >>>>> YKNewTask(CTask, (void *)&CStk[256], 2); 
	mov	al, 2
	push	ax
	mov	ax, (CStk+512)
	push	ax
	mov	ax, CTask
	push	ax
	call	YKNewTask
	add	sp, 6
	; >>>>> Line:	43
	; >>>>> printString("Task A is still running! Oh no! Task A was supposed to stop.\n"); 
	mov	ax, L_lab4b_app_9
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	44
	; >>>>> exit(0); 
	xor	al, al
	push	ax
	call	exit
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_lab4b_app_10:
	push	bp
	mov	bp, sp
	jmp	L_lab4b_app_11
L_lab4b_app_13:
	DB	"Task B started! Oh no! Task B wasn't supposed to run.",0xA,0
	ALIGN	2
BTask:
	; >>>>> Line:	48
	; >>>>> { 
	jmp	L_lab4b_app_14
L_lab4b_app_15:
	; >>>>> Line:	49
	; >>>>> printString("Task B started! Oh no! Task B wasn't supposed to run.\n"); 
	mov	ax, L_lab4b_app_13
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	50
	; >>>>> exit(0); 
	xor	al, al
	push	ax
	call	exit
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_lab4b_app_14:
	push	bp
	mov	bp, sp
	jmp	L_lab4b_app_15
L_lab4b_app_19:
	DB	"Executing in task C.",0xA,0
L_lab4b_app_18:
	DB	" context switches!",0xA,0
L_lab4b_app_17:
	DB	"Task C started after ",0
	ALIGN	2
CTask:
	; >>>>> Line:	54
	; >>>>> { 
	jmp	L_lab4b_app_20
L_lab4b_app_21:
	; >>>>> Line:	58
	; >>>>> YKEnterMutex(); 
	call	YKEnterMutex
	; >>>>> Line:	59
	; >>>>> numCtxSwi 
	mov	ax, word [YKCtxSwCount]
	mov	word [bp-4], ax
	; >>>>> Line:	60
	; >>>>> YKExitMutex(); 
	call	YKExitMutex
	; >>>>> Line:	62
	; >>>>> printString("Task C started after "); 
	mov	ax, L_lab4b_app_17
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	63
	; >>>>> printUInt(numCtxSwitches); 
	push	word [bp-4]
	call	printUInt
	add	sp, 2
	; >>>>> Line:	64
	; >>>>> printString(" context switches!\n"); 
	mov	ax, L_lab4b_app_18
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	66
	; >>>>> while (1) 
	jmp	L_lab4b_app_23
L_lab4b_app_22:
	; >>>>> Line:	68
	; >>>>> printString("Executing in task C.\n"); 
	mov	ax, L_lab4b_app_19
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	69
	; >>>>> for(count = 0; count < 5000; count++); 
	mov	word [bp-2], 0
	jmp	L_lab4b_app_26
L_lab4b_app_25:
L_lab4b_app_28:
	inc	word [bp-2]
L_lab4b_app_26:
	cmp	word [bp-2], 5000
	jl	L_lab4b_app_25
L_lab4b_app_27:
L_lab4b_app_23:
	jmp	L_lab4b_app_22
L_lab4b_app_24:
	mov	sp, bp
	pop	bp
	ret
L_lab4b_app_20:
	push	bp
	mov	bp, sp
	sub	sp, 4
	jmp	L_lab4b_app_21
	ALIGN	2
AStk:
	TIMES	512 db 0
BStk:
	TIMES	512 db 0
CStk:
	TIMES	512 db 0
