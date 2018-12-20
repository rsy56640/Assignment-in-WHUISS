.386
.model flat,stdcall
option casemap:none

include     \masm32\include\windows.inc
include     \masm32\include\kernel32.inc
include     \masm32\include\user32.inc

printf      PROTO C :DWORD, :VARARG

includelib  \masm32\lib\kernel32.lib
includelib  \masm32\lib\user32.lib
includelib  \masm32\lib\msvcrt.lib

.data

format      db	"solution%d: %d %d %d %d %d %d %d %d", 10, 0

.code

start:

    call    main
    invoke  ExitProcess,0   

main PROC
	push	ebp
	mov		ebp,	esp
	sub		esp,	32				; int queen[8]
	mov		ebx,	0				; global counter
	mov		edi,	esp				; queen
	mov		esi,	-1				; start position
	call	eight_queen
	mov		eax,	0
	mov		esp,	ebp
	pop		ebp
    ret
main ENDP


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;    void eight_queen(int* queen, int pos);             ;
;        edi points to queen[0].                        ;
;        esi denotes position that has been determined. ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
eight_queen PROC
	cmp		esi,	7
	jne		compute					; if pos == 7 print and return, else compute
print_result_and_return:
	inc		ebx						; counter++
	push	DWORD PTR [edi+28]
	push	DWORD PTR [edi+24]
	push	DWORD PTR [edi+20]
	push	DWORD PTR [edi+16]
	push	DWORD PTR [edi+12]
	push	DWORD PTR [edi+8]
	push	DWORD PTR [edi+4]
	push	DWORD PTR [edi+0]
	push	ebx
    push    OFFSET format			; format string
    call    printf
	add		esp,	28h				; notice here, debug for too long
	jmp		return
compute:
	mov		ecx,	0				; ecx = 0...7
eight_queen_iter:
	mov		edx,	ecx	
	call	judge					; judge whether the position can be stuffed with edx
	cmp		eax,	1
	jne		no_recur				; not satisfied, go to next turn
	push	ecx
	push	esi						; store pos
	inc		esi						; _pos = pos++
	mov		DWORD PTR [edi+esi*4],	ecx	; queen[_pos] = ecx, set the position to ecx
	call	eight_queen				; eight_queen(queen, pos+1)
	pop		esi						; recover pos
	pop		ecx
no_recur:
	inc		ecx						; ecx++
	cmp		ecx,	8
	jne		eight_queen_iter
return:
	ret
eight_queen ENDP


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  int judge(int* queen, int pos, int i)       ;
;      return 1 if pos+1 can be stuffed with i ;	if(queen[edx] == i   // row collision
;      %edi points to queen[0]                 ;		||               // diagonal collision
;      %esi denotes the position before        ;		pos+1-edx == abs(i-queen[edx]))
;      %edx denotes the i                      ;			return false;
;      %esi and %edx can be changed            ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
judge PROC
	push	ebp
	mov		ebp,	esp
	sub		esp,	8
	mov		DWORD PTR [ebp-4],	edx	; store i
	mov		eax,	1
	mov		edx,	0				; edx = 0...pos
	cmp		edx,	esi
	jg		return
judge_iter:
	mov		eax,	DWORD PTR [edi+edx*4]	; eax = queen[edx]
	sub		eax,	DWORD PTR [ebp-4]	; eax = i-queen[edx]
	cmp		eax,	0
	je		return_false
	jg		positive
abs:
	not		eax						; eax = ~eax
	inc		eax						; eax += 1
positive:							; eax = abs(i-queen[edx]))
	mov		DWORD PTR [ebp-8],	esi	; pos
	inc		DWORD PTR [ebp-8]		; pos+1
	sub		DWORD PTR [ebp-8],	edx	; pos+1-edx
	cmp		DWORD PTR [ebp-8],	eax	;
	je		return_false
	inc		edx
	mov		eax,	esi
	inc		eax						; eax = pos+1
	cmp		edx,	eax				; edx is more than pos
	mov		eax,	1
	je		return					; return true
	jmp		judge_iter				; edx = 0...pos
return_false:
	mov		eax,	0
	jmp		return
return:
	mov		esp,	ebp
	pop		ebp
	ret
judge ENDP


END start