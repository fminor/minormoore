; Generated by c86 (BYU-NASM) 5.1 (beta) from yakc.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
	ALIGN	2
YKInitialize:
	; >>>>> Line:	27
	; >>>>> void YKInitialize() { 
	jmp	L_yakc_1
L_yakc_2:
	; >>>>> Line:	30
	; >>>>> YKCtxSwCount = 0; 
	mov	word [YKCtxSwCount], 0
	; >>>>> Line:	31
	; >>>>> YKIdleCount = 0; 
	mov	word [YKIdleCount], 0
	; >>>>> Line:	32
	; >>>>> YKTickNum = 0; 
	mov	word [YKTickNum], 0
	; >>>>> Line:	34
	; >>>>> YKRdyList = 0; 
	mov	word [YKRdyList], 0
	; >>>>> Line:	35
	; >>>>> YKSuspList = 0; 
	mov	word [YKSuspList], 0
	; >>>>> Line:	36
	; >>>>> YKAvailTCBList = YKTCBArray; 
	mov	word [YKAvailTCBList], YKTCBArray
	; >>>>> Line:	37
	; >>>>> YKTCBArray[0].prev = 0; 
	mov	word [(10+YKTCBArray)], 0
	; >>>>> Line:	38
	; >>>>> for(i = 0; i < 3 + 1; i++){ 
	mov	word [bp-2], 0
	jmp	L_yakc_4
L_yakc_3:
	; >>>>> Line:	39
	; >>>>> YKTCBArray[i].next = &YKTCBArray[i + 1]; 
	mov	ax, word [bp-2]
	inc	ax
	mov	cx, 12
	imul	cx
	add	ax, YKTCBArray
	push	ax
	mov	ax, word [bp-2]
	mov	cx, 12
	imul	cx
	mov	dx, ax
	add	dx, YKTCBArray
	mov	si, dx
	add	si, 8
	pop	ax
	mov	word [si], ax
	; >>>>> Line:	40
	; >>>>> YKTCBArray[i + 1].prev = &YKTCBArray[i]; 
	mov	ax, word [bp-2]
	mov	cx, 12
	imul	cx
	add	ax, YKTCBArray
	mov	dx, word [bp-2]
	inc	dx
	push	ax
	mov	ax, dx
	mov	cx, 12
	imul	cx
	mov	dx, ax
	add	dx, YKTCBArray
	mov	si, dx
	add	si, 10
	pop	ax
	mov	word [si], ax
L_yakc_6:
	inc	word [bp-2]
L_yakc_4:
	cmp	word [bp-2], 4
	jl	L_yakc_3
L_yakc_5:
	; >>>>> Line:	43
	; >>>>> YKNewTask(YKIdle, (void*)&IStk[255],255); 
	mov	al, 255
	push	ax
	mov	ax, (IStk+510)
	push	ax
	mov	ax, YKIdle
	push	ax
	call	YKNewTask
	add	sp, 6
	mov	sp, bp
	pop	bp
	ret
L_yakc_1:
	push	bp
	mov	bp, sp
	sub	sp, 4
	jmp	L_yakc_2
	ALIGN	2
YKNewTask:
	; >>>>> Line:	48
	; >>>>> void YKNewTask(void (*task)(void), void*taskStack, unsigned char priority) { 
	jmp	L_yakc_8
L_yakc_9:
	; >>>>> Line:	52
	; >>>>> new_task->priority = priority; 
	mov	ax, word [YKAvailTCBList]
	add	word [YKAvailTCBList], 12
	mov	word [bp-10], ax
	; >>>>> Line:	52
	; >>>>> new_task->priority = priority; 
	mov	si, word [bp-10]
	add	si, 4
	mov	al, byte [bp+8]
	mov	byte [si], al
	; >>>>> Line:	53
	; >>>>> new_task->state = 0; 
	mov	si, word [bp-10]
	add	si, 2
	mov	word [si], 0
	; >>>>> Line:	54
	; >>>>> u 
	mov	si, word [bp-10]
	mov	ax, word [bp+6]
	mov	word [si], ax
	; >>>>> Line:	55
	; >>>>> new_task->delay = 0; 
	mov	si, word [bp-10]
	add	si, 6
	mov	word [si], 0
	; >>>>> Line:	56
	; >>>>> YKRdyList = queue(YKRdyList,new_task); 
	push	word [bp-10]
	push	word [YKRdyList]
	call	queue
	add	sp, 4
	mov	word [YKRdyList], ax
	; >>>>> Line:	57
	; >>>>> ip = (int) task & 0xFFFF; 
	mov	ax, word [bp+4]
	and	ax, 65535
	mov	word [bp-4], ax
	; >>>>> Line:	58
	; >>>>> cs = 0; 
	mov	word [bp-2], 0
	; >>>>> Line:	59
	; >>>>> sp = (int) taskStack & 0xFFFF; 
	mov	ax, word [bp+6]
	and	ax, 65535
	mov	word [bp-8], ax
	; >>>>> Line:	60
	; >>>>> ss = 0; 
	mov	word [bp-6], 0
	; >>>>> Line:	61
	; >>>>> initStack(cs,ip,ss,sp); 
	push	word [bp-8]
	push	word [bp-6]
	push	word [bp-4]
	push	word [bp-2]
	call	initStack
	add	sp, 8
	mov	sp, bp
	pop	bp
	ret
L_yakc_8:
	push	bp
	mov	bp, sp
	sub	sp, 10
	jmp	L_yakc_9
	ALIGN	2
YKRun:
	; >>>>> Line:	73
	; >>>>> void YKRun() { 
	jmp	L_yakc_11
L_yakc_12:
	; >>>>> Line:	75
	; >>>>> } 
	mov	sp, bp
	pop	bp
	ret
L_yakc_11:
	push	bp
	mov	bp, sp
	jmp	L_yakc_12
	ALIGN	2
YKDelayTask:
	; >>>>> Line:	77
	; >>>>> void YKDelayTask(int count) { 
	jmp	L_yakc_14
L_yakc_15:
	; >>>>> Line:	81
	; >>>>> } 
	mov	sp, bp
	pop	bp
	ret
L_yakc_14:
	push	bp
	mov	bp, sp
	jmp	L_yakc_15
	ALIGN	2
YKEnterMutex:
	; >>>>> Line:	83
	; >>>>> void YKEnterMutex() { 
	jmp	L_yakc_17
L_yakc_18:
	; >>>>> Line:	87
	; >>>>> } 
	mov	sp, bp
	pop	bp
	ret
L_yakc_17:
	push	bp
	mov	bp, sp
	jmp	L_yakc_18
	ALIGN	2
YKExitMutex:
	; >>>>> Line:	89
	; >>>>> void YKExitMutex() { 
	jmp	L_yakc_20
L_yakc_21:
	; >>>>> Line:	93
	; >>>>> } 
	mov	sp, bp
	pop	bp
	ret
L_yakc_20:
	push	bp
	mov	bp, sp
	jmp	L_yakc_21
	ALIGN	2
YKEnterISR:
	; >>>>> Line:	96
	; >>>>> void YKEnterISR() { 
	jmp	L_yakc_23
L_yakc_24:
	; >>>>> Line:	98
	; >>>>> } 
	mov	sp, bp
	pop	bp
	ret
L_yakc_23:
	push	bp
	mov	bp, sp
	jmp	L_yakc_24
	ALIGN	2
YKExitISR:
	; >>>>> Line:	100
	; >>>>> void YKExitISR() { 
	jmp	L_yakc_26
L_yakc_27:
	; >>>>> Line:	102
	; >>>>> } 
	mov	sp, bp
	pop	bp
	ret
L_yakc_26:
	push	bp
	mov	bp, sp
	jmp	L_yakc_27
	ALIGN	2
YKScheduler:
	; >>>>> Line:	105
	; >>>>> void YKScheduler() { 
	jmp	L_yakc_29
L_yakc_30:
	; >>>>> Line:	107
	; >>>>> YKDispatcher(next); 
	push	word [YKRdyList]
	call	dequeue
	add	sp, 2
	mov	word [bp-2], ax
	; >>>>> Line:	107
	; >>>>> YKDispatcher(next); 
	push	word [bp-2]
	call	YKDispatcher
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_yakc_29:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_30
	ALIGN	2
YKDispatcher:
	; >>>>> Line:	111
	; >>>>> void YKDispatcher(TCBptr next) { 
	jmp	L_yakc_32
L_yakc_33:
	; >>>>> Line:	119
	; >>>>> } 
	mov	sp, bp
	pop	bp
	ret
L_yakc_32:
	push	bp
	mov	bp, sp
	jmp	L_yakc_33
	ALIGN	2
YKTickHandler:
	; >>>>> Line:	121
	; >>>>> void YKTickHandler() { 
	jmp	L_yakc_35
L_yakc_36:
	; >>>>> Line:	123
	; >>>>> ask->ne 
	mov	si, word [YKSuspList]
	add	si, 6
	mov	ax, word [si]
	dec	ax
	mov	si, word [YKSuspList]
	add	si, 6
	mov	word [si], ax
	; >>>>> Line:	124
	; >>>>> if(YKSuspList->delay == 0){ 
	mov	si, word [YKSuspList]
	add	si, 6
	mov	ax, word [si]
	test	ax, ax
	jne	L_yakc_37
	; >>>>> Line:	126
	; >>>>> YKRdyList = queue(YKRdyList,task); 
	push	word [YKSuspList]
	call	dequeue
	add	sp, 2
	mov	word [bp-2], ax
	; >>>>> Line:	126
	; >>>>> YKRdyList = queue(YKRdyList,task); 
	push	word [bp-2]
	push	word [YKRdyList]
	call	queue
	add	sp, 4
	mov	word [YKRdyList], ax
L_yakc_37:
	mov	sp, bp
	pop	bp
	ret
L_yakc_35:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_36
	ALIGN	2
YKIdle:
	; >>>>> Line:	130
	; >>>>> void YKIdle() { 
	jmp	L_yakc_39
L_yakc_40:
L_yakc_41:
	; >>>>> Line:	131
	; >>>>> while(1); 
L_yakc_42:
	jmp	L_yakc_41
L_yakc_43:
	mov	sp, bp
	pop	bp
	ret
L_yakc_39:
	push	bp
	mov	bp, sp
	jmp	L_yakc_40
	ALIGN	2
queue:
	; >>>>> Line:	134
	; >>>>> TCBptr queue(TCBptr queue_head, TCBptr task){ 
	jmp	L_yakc_45
L_yakc_46:
	; >>>>> Line:	135
	; >>>>> if(queue_head == 0){ 
	mov	ax, word [bp+4]
	test	ax, ax
	jne	L_yakc_47
	; >>>>> Line:	136
	; >>>>> return queue_head; 
	jmp	L_yakc_48
L_yakc_47:
	; >>>>> Line:	138
	; >>>>> if(queue_head->priority > task->priority){ 
	mov	si, word [bp+4]
	add	si, 4
	mov	di, word [bp+6]
	add	di, 4
	mov	al, byte [di]
	cmp	al, byte [si]
	jae	L_yakc_49
	; >>>>> Line:	139
	; >>>>> task->next = queue_head; 
	mov	si, word [bp+6]
	add	si, 8
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	140
	; >>>>> queue_head->prev = task; 
	mov	si, word [bp+4]
	add	si, 10
	mov	ax, word [bp+6]
	mov	word [si], ax
	; >>>>> Line:	141
	; >>>>> queue_head = task; 
	mov	ax, word [bp+6]
	mov	word [bp+4], ax
	jmp	L_yakc_50
L_yakc_49:
	; >>>>> Line:	144
	; >>>>> while(temp->next->priority < task->priority) { 
	mov	ax, word [bp+4]
	mov	word [bp-2], ax
	; >>>>> Line:	144
	; >>>>> while(temp->next->priority < task->priority) { 
	jmp	L_yakc_52
L_yakc_51:
	; >>>>> Line:	145
	; >>>>> temp = temp->next; 
	mov	si, word [bp-2]
	add	si, 8
	mov	ax, word [si]
	mov	word [bp-2], ax
L_yakc_52:
	mov	si, word [bp-2]
	add	si, 8
	mov	si, word [si]
	add	si, 4
	mov	di, word [bp+6]
	add	di, 4
	mov	al, byte [di]
	cmp	al, byte [si]
	ja	L_yakc_51
L_yakc_53:
	; >>>>> Line:	147
	; >>>>> task->ne 
	mov	si, word [bp-2]
	add	si, 8
	mov	di, word [bp+6]
	add	di, 8
	mov	ax, word [si]
	mov	word [di], ax
	; >>>>> Line:	148
	; >>>>> task->next->prev = task; 
	mov	si, word [bp+6]
	add	si, 8
	mov	si, word [si]
	add	si, 10
	mov	ax, word [bp+6]
	mov	word [si], ax
	; >>>>> Line:	149
	; >>>>> task->prev = temp; 
	mov	si, word [bp+6]
	add	si, 10
	mov	ax, word [bp-2]
	mov	word [si], ax
	; >>>>> Line:	150
	; >>>>> temp->next = task; 
	mov	si, word [bp-2]
	add	si, 8
	mov	ax, word [bp+6]
	mov	word [si], ax
L_yakc_50:
	; >>>>> Line:	152
	; >>>>> return queue_head; 
	mov	ax, word [bp+4]
L_yakc_48:
	mov	sp, bp
	pop	bp
	ret
L_yakc_45:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_46
	ALIGN	2
dequeue:
	; >>>>> Line:	155
	; >>>>> TCBptr dequeue(TCBptr queue_head){ 
	jmp	L_yakc_55
L_yakc_56:
	; >>>>> Line:	157
	; >>>>> queue_head = queue_head->next; 
	mov	ax, word [bp+4]
	mov	word [bp-2], ax
	; >>>>> Line:	157
	; >>>>> queue_head = queue_head->next; 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [si]
	mov	word [bp+4], ax
	; >>>>> Line:	158
	; >>>>> return next; 
	mov	ax, word [bp-2]
L_yakc_57:
	mov	sp, bp
	pop	bp
	ret
L_yakc_55:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_56
	ALIGN	2
suspendTask:
	; >>>>> Line:	161
	; >>>>> void suspendTask(TCBptr task){ 
	jmp	L_yakc_59
L_yakc_60:
	; >>>>> Line:	163
	; >>>>> if(task->delay < YKSuspList->delay){ 
	mov	word [bp-2], 0
	; >>>>> Line:	163
	; >>>>> if(task->delay < YKSuspList->delay){ 
	mov	si, word [bp+4]
	add	si, 6
	mov	di, word [YKSuspList]
	add	di, 6
	mov	ax, word [di]
	cmp	ax, word [si]
	jle	L_yakc_61
	; >>>>> Line:	164
	; >>>>> YKSuspList->delay -= task->delay; 
	mov	si, word [YKSuspList]
	add	si, 6
	mov	di, word [bp+4]
	add	di, 6
	mov	ax, word [di]
	sub	word [si], ax
	; >>>>> Line:	165
	; >>>>> YKSuspList = task; 
	mov	ax, word [bp+4]
	mov	word [YKSuspList], ax
	jmp	L_yakc_62
L_yakc_61:
	; >>>>> Line:	169
	; >>>>> temp = YKSuspList; 
	mov	ax, word [YKSuspList]
	mov	word [bp-2], ax
	; >>>>> Line:	170
	; >>>>> while(temp->next != 0 && temp->delay < task->delay){ 
	jmp	L_yakc_64
L_yakc_63:
	; >>>>> Line:	171
	; >>>>> task->delay -= temp->delay; 
	mov	si, word [bp+4]
	add	si, 6
	mov	di, word [bp-2]
	add	di, 6
	mov	ax, word [di]
	sub	word [si], ax
	; >>>>> Line:	172
	; >>>>> temp = te 
	mov	si, word [bp-2]
	add	si, 8
	mov	ax, word [si]
	mov	word [bp-2], ax
L_yakc_64:
	mov	si, word [bp-2]
	add	si, 8
	mov	ax, word [si]
	test	ax, ax
	je	L_yakc_66
	mov	si, word [bp-2]
	add	si, 6
	mov	di, word [bp+4]
	add	di, 6
	mov	ax, word [di]
	cmp	ax, word [si]
	jg	L_yakc_63
L_yakc_66:
L_yakc_65:
	; >>>>> Line:	174
	; >>>>> if(temp->delay < task->delay) { 
	mov	si, word [bp-2]
	add	si, 6
	mov	di, word [bp+4]
	add	di, 6
	mov	ax, word [di]
	cmp	ax, word [si]
	jle	L_yakc_67
	; >>>>> Line:	175
	; >>>>> task->delay -= temp->delay; 
	mov	si, word [bp+4]
	add	si, 6
	mov	di, word [bp-2]
	add	di, 6
	mov	ax, word [di]
	sub	word [si], ax
	; >>>>> Line:	176
	; >>>>> temp->next = task; 
	mov	si, word [bp-2]
	add	si, 8
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	177
	; >>>>> task->prev = temp; 
	mov	si, word [bp+4]
	add	si, 10
	mov	ax, word [bp-2]
	mov	word [si], ax
	jmp	L_yakc_68
L_yakc_67:
	; >>>>> Line:	179
	; >>>>> task->prev = temp->prev; 
	mov	si, word [bp-2]
	add	si, 10
	mov	di, word [bp+4]
	add	di, 10
	mov	ax, word [si]
	mov	word [di], ax
	; >>>>> Line:	180
	; >>>>> task->next = temp; 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [bp-2]
	mov	word [si], ax
	; >>>>> Line:	181
	; >>>>> temp->prev = task; 
	mov	si, word [bp-2]
	add	si, 10
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	182
	; >>>>> temp->delay -= task->delay; 
	mov	si, word [bp-2]
	add	si, 6
	mov	di, word [bp+4]
	add	di, 6
	mov	ax, word [di]
	sub	word [si], ax
L_yakc_68:
L_yakc_62:
	mov	sp, bp
	pop	bp
	ret
L_yakc_59:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_60
	ALIGN	2
YKRdyList:
	TIMES	2 db 0
YKSuspList:
	TIMES	2 db 0
YKAvailTCBList:
	TIMES	2 db 0
YKTCBArray:
	TIMES	48 db 0
YKCtxSwCount:
	TIMES	2 db 0
YKIdleCount:
	TIMES	2 db 0
YKTickNum:
	TIMES	2 db 0
IStk:
	TIMES	510 db 0
