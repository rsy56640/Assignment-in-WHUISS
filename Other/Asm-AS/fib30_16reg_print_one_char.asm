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

format1      db	"fib%hu: ", 0
format2      db	"%hu", 0
format3      db	10, 0

.code

start:

    call    main
    invoke  ExitProcess,0   

main PROC
	call fib
    ret
main ENDP

; compute first 30 fibonacci numbers
; !!!!!!!!!!!!!!!!!!  using 16-bit register and print 1 char each time  !!!!!!!!!!!!!!!!!!
fib PROC
	push	ebp
	mov		ebp,	esp
	sub		esp,	10
	mov		WORD PTR [esp+0],	1	; low-16 a
	mov		WORD PTR [esp+2],	1	; low-16 b
	mov		WORD PTR [esp+4],	0	; high-16 a
	mov		WORD PTR [esp+6],	0	; high-16 b
	mov		cx,		1				; cx = 1...30
fib_iter:
	mov		si,		WORD PTR [esp+0]
	mov		di,		WORD PTR [esp+4]
	mov		WORD PTR [esp+8],	cx	; store counter
	call	print_fib
	mov		cx,		WORD PTR [esp+8]; take counter
	mov		bx,		WORD PTR [esp+0]
	mov		dx,		WORD PTR [esp+2]
	add		bx,		dx				; b, a
	jc		low_carry				; if add bx, dx set carry flag
	jmp		no_carry
low_carry:
	inc		WORD PTR [esp+4]
no_carry:
	xor		bx,		dx				; b^a, a
	xor		dx,		bx				; b^a, b
	xor		bx,		dx				; a, b
	mov		WORD PTR [esp+0],	bx
	mov		WORD PTR [esp+2],	dx
	mov		bx,		WORD PTR [esp+4]
	mov		dx,		WORD PTR [esp+6]
	add		bx,		dx				; b, a
	xor		bx,		dx				; b^a, a
	xor		dx,		bx				; b^a, b
	xor		bx,		dx				; a, b
	mov		WORD PTR [esp+4],	bx
	mov		WORD PTR [esp+6],	dx
	inc		cx						; next turn
	cmp		cx,		31
	jne		fib_iter
fib_quit:
	mov		esp,	ebp
	pop		ebp
	ret
fib ENDP


; %cx means the counter
; %di means the high-16-bit ; x = x4*10^4 + x3*10^3 + x2*10^2 + x1*10^1 + x0*10^0
; %si means the low-16-bit  ; y = y4*10^4 + y3*10^3 + y2*10^2 + y1*10^1 + y0*10^0
; result = x*2^16 + y = 65536x + y
; coefficients for decimal
; 10^8 -- 6*x4
; 10^7 -- 5*x4 + 6*x3
; 10^6 -- 5*x4 + 5*x3 + 6*x2
; 10^5 -- 3*x4 + 5*x3 + 5*x2 + 6*x1
; 10^4 -- 6*x4 + 3*x3 + 5*x2 + 5*x1 + 6*x0 + y4
; 10^3 -- 6*x3 + 3*x2 + 5*x1 + 5*x0 + y3
; 10^2 -- 6*x2 + 3*x1 + 5*x0 + y2
; 10^1 -- 6*x1 + 3*x0 + y1
; 10^0 -- 6*x0 + y0
print_fib PROC
	push	ebp
	mov		ebp,	esp
	sub		esp,	48
	; [esp+0]  - [esp+8]  denote y0 - y4
	; [esp+10] - [esp+18] denote x0 - x4
	; [esp+20] - [esp+38] denote t0 - t9
	; [esp+40] temp0
	; [esp+42] temp1
	; [esp+44] temp2
	; [esp+46] temp3

	; (dx, ax) / xx = ax ... dx
	mov		bx,		10
	mov		ax,		si				; ax = y
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+0],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+2],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+4],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+6],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+8],	dx	;

	mov		ax,		di				; ax = x
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+10],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+12],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+14],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+16],	dx	;
	xor		dx,		dx
	div		bx
	mov		WORD PTR [esp+18],	dx	;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;; printf("fib%hu", counter);
	push	cx
	push	OFFSET format1
	call	printf
	add		esp,	6

;;;;;;;;;;;;;;;;;;;;;;;;;;;;; compute t0 - t8
;;;;;;;;;;;;;;;;;;;;;;;;;;;;; can use %ax, %bx, %dx, %di, %si
;;;;;;;;;;;;;;;;;;;;;;;;;;;;; [esp+0]  - [esp+8]  denote y0 - y4
;;;;;;;;;;;;;;;;;;;;;;;;;;;;; [esp+10] - [esp+18] denote x0 - x4
;;;;;;;;;;;;;;;;;;;;;;;;;;;;; [esp+20] - [esp+38] denote t0 - t9
$tmp0 = 40 ;;;;;;;;;;;;;;;;;; [esp+40] temp0 = 3*x0, 5*x0, 3*x4, 5*x4
$tmp1 = 42 ;;;;;;;;;;;;;;;;;; [esp+42] temp1 = 3*x1, 5*x1
$tmp2 = 44 ;;;;;;;;;;;;;;;;;; [esp+44] temp2 = 3*x2, 5*x2
$tmp3 = 46 ;;;;;;;;;;;;;;;;;; [esp+46] temp3 = 3*x3, 5*x3

	mov		ax,		WORD PTR [esp+10]	; x0
	mov		bx,		ax
	sal		ax,		1					; 2*x0
	add		ax,		bx					; 3*x0
	mov		WORD PTR [esp+$tmp0],	ax	; temp0 = 3*x0
	sal		ax,		1					; 6*x0
	add		ax,		WORD PTR [esp+0]	; 6*x0 + y0
	mov		si,		0					; init
	mov		di,		0					; init
	call	update_carry
	mov		WORD PTR [esp+20],	ax		; store t0


	mov		ax,		WORD PTR [esp+12]	; x1
	mov		bx,		ax
	sal		ax,		1					; 2*x1
	add		ax,		bx					; 3*x1
	mov		WORD PTR [esp+$tmp1],	ax	; temp1 = 3*x1
	sal		ax,		1					; 6*x1
	add		ax,		WORD PTR [esp+$tmp0]; 6*x1 + 3*x0
	add		ax,		WORD PTR [esp+2]	; 6*x1 + 3*x0 + y1
	call	update_carry
	mov		WORD PTR [esp+22],	ax		; store t1


	mov		ax,		WORD PTR [esp+14]	; x2
	mov		bx,		ax
	sal		ax,		1					; 2*x2
	add		ax,		bx					; 3*x2
	mov		WORD PTR [esp+$tmp2],	ax	; temp2 = 3*x2
	sal		ax,		1					; 6*x2
	add		ax,		WORD PTR [esp+$tmp1]; 6*x2 + 3*x1
	mov		bx,		WORD PTR [esp+10]	; x0
	sal		bx,		1					; 2*x0
	add		bx,		WORD PTR [esp+$tmp0]; 5*x0
	mov		WORD PTR [esp+$tmp0],	bx	; temp0 = 5*x0
	add		ax,		bx					; 6*x2 + 3*x1 + 5*x0
	add		ax,		WORD PTR [esp+4]	; 6*x2 + 3*x1 + 5*x0 + y2	
	call	update_carry
	mov		WORD PTR [esp+24],	ax		; store t2


	mov		ax,		WORD PTR [esp+16]	; x3
	mov		bx,		ax
	sal		ax,		1					; 2*x3
	add		ax,		bx					; 3*x3
	mov		WORD PTR [esp+$tmp3],	ax	; temp3 = 3*x3
	sal		ax,		1					; 6*x3
	add		ax,		WORD PTR [esp+$tmp2]; 6*x3 + 3*x2
	add		ax,		WORD PTR [esp+$tmp0]; 6*x3 + 3*x2 + 5*x0
	mov		bx,		WORD PTR [esp+12]	; x1
	sal		bx,		1					; 2*x1
	add		bx,		WORD PTR [esp+$tmp1]; 5*x1
	mov		WORD PTR [esp+$tmp1],	bx	; temp1 = 5*x1
	add		ax,		bx					; 6*x3 + 3*x2 + 5*x1 + 5*x0
	add		ax,		WORD PTR [esp+6]	; 6*x3 + 3*x2 + 5*x1 + 5*x0 + y3
	call	update_carry
	mov		WORD PTR [esp+26],	ax		; store t3


	mov		ax,		WORD PTR [esp+18]	; x4
	mov		bx,		ax
	sal		ax,		1					; 2*x4
	add		ax,		bx					; 3*x4
	mov		bx,		ax					; bx = 3*x4
	sal		ax,		1					; 6*x4
	add		ax,		WORD PTR [esp+$tmp3]; 6*x4 + 3*x3
	add		ax,		WORD PTR [esp+$tmp0]; 6*x4 + 3*x3 + 5*x0
	add		ax,		WORD PTR [esp+10]	; 6*x4 + 3*x3 + 6*x0
	mov		WORD PTR [esp+$tmp0],	bx	; temp0 = 3*x4
	mov		bx,		WORD PTR [esp+14]	; x2
	sal		bx,		1					; 2*x2
	add		bx,		WORD PTR [esp+$tmp2]; 5*x2
	mov		WORD PTR [esp+$tmp2],	bx	; temp2 = 5*x2
	add		ax,		bx					; 6*x4 + 3*x3 + 5*x2 + 6*x0
	add		ax,		WORD PTR [esp+$tmp1]; 6*x4 + 3*x3 + 5*x2 + 5*x1 + 6*x0
	add		ax,		WORD PTR [esp+8]	; 6*x4 + 3*x3 + 5*x2 + 5*x1 + 6*x0 + y4
	call	update_carry
	mov		WORD PTR [esp+28],	ax		; store t4


	mov		ax,		WORD PTR [esp+$tmp0]; 3*x4
	mov		bx,		WORD PTR [esp+16]	; x3
	sal		bx,		1					; 2*x3
	add		bx,		WORD PTR [esp+$tmp3]; 5*x3
	mov		WORD PTR [esp+$tmp3],	bx	; temp3 = 5*x3
	add		ax,		bx					; 3*x4 + 5*x3
	add		ax,		WORD PTR [esp+$tmp2]; 3*x4 + 5*x3 + 5*x2
	add		ax,		WORD PTR [esp+$tmp1]; 3*x4 + 5*x3 + 5*x2 + 5*x1
	add		ax,		WORD PTR [esp+12]	; 3*x4 + 5*x3 + 5*x2 + 6*x1
	call	update_carry
	mov		WORD PTR [esp+30],	ax		; store t5


	mov		ax,		WORD PTR [esp+$tmp0]; 3*x4
	mov		bx,		WORD PTR [esp+18]	; x4
	sal		bx,		1					; 2*x4
	add		ax,		bx					; 5*x4
	mov		WORD PTR [esp+$tmp0],	ax	; temp0 = 5*x4
	add		ax,		WORD PTR [esp+$tmp3]; 5*x4 + 5*x3
	add		ax,		WORD PTR [esp+$tmp2]; 5*x4 + 5*x3 + 5*x2
	add		ax,		WORD PTR [esp+14]	; 5*x4 + 5*x3 + 6*x2
	call	update_carry
	mov		WORD PTR [esp+32],	ax		; store t6


	mov		ax,		WORD PTR [esp+$tmp0]; 5*x4
	mov		bx,		ax
	add		ax,		WORD PTR [esp+$tmp3]; 5*x4 + 5*x3
	add		ax,		WORD PTR [esp+16]	; 5*x4 + 6*x3
	call	update_carry
	mov		WORD PTR [esp+34],	ax		; store t7


	mov		ax,		WORD PTR [esp+$tmp0]; 5*x4
	add		ax,		WORD PTR [esp+18]	; 6*x4
	call	update_carry
	mov		WORD PTR [esp+36],	ax		; store t8


	mov		WORD PTR [esp+38],	si		; store t9


;;;;;;;;;;;;;;;;;; %di has no use from here (in fact, %di must be 0, 2^32 has no t10)
	mov		eax,	0				; must use %eax to calculate esp
	mov		di,		10
find_not_zero:
	dec		di
	mov		ax,		di
	sal		ax,		1					; %ax = 2*%di
	add		ax,		14H					; %ax = 20 + 2*%di, 20 is base address of t0
	cmp		WORD PTR [esp+eax],	0		; must use %eax instead of %ax with %esp
	je		find_not_zero

;;;;;;;;;;;;;;;;;; %di denotes the first number not zero
printf_one_char:
	mov		ax,		di
	sal		ax,		1				; ax = di*2
	add		ax,		14H
	push	WORD PTR [esp+eax]		; must use %eax instead of %ax with %esp
	push	OFFSET format2
	call	printf
	add		esp,	6
	dec		di
	cmp		di,		-1
	jne		printf_one_char
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;; printf("\n");
	push	OFFSET format3
	call	printf
	add		esp,	4

	mov		esp,	ebp
	pop		ebp
	ret
print_fib ENDP


; %ax denotes the number < 100 (xy)
;;;;;;;;;;;;;;;;;;;;;;;;;       ;;;;;;;;;;;;;;;;;;;;;;;       ;;;;;;;;;;;;;;;;;;;;;;
;  ; %di ; %si, xy(%ax) ;   ->  ; %di ; %si ; xy(%ax) ;   ->  ; %di ; %si ; y(%ax) ;
;;;;;;;;;;;;;;;;;;;;;;;;;       ;;;;;;;;;;;;;;;;;;;;;;;       ;;;;;;;;;;;;;;;;;;;;;;
update_carry PROC
	add		ax,		si				; ax += si(r1)
	mov		si,		di				; next
	mov		di,		0				; next

	mov		bx,		10

	xor		dx,		dx
	div		bx
	mov		cx,		dx				; remainder as y
	
	add		ax,		si				; (%si + x) / 10
	xor		dx,		dx
	div		bx
	mov		si,		dx				; (%si + x) % 10
	add		di,		ax				; %di += (%si + x) / 10 ; notice that %di must < 10, so safe.

	mov		ax,		cx
	ret
update_carry ENDP


END start