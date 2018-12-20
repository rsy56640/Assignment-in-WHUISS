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

format      db	"prime%d = %d", 10, 0

.code

start:

    call    main
    invoke  ExitProcess,0   

main PROC
	push	ebp
	mov		ebp,	esp
	sub		esp,	8
	mov		DWORD PTR [ebp-4], 100		; from 100 to 2
	mov		DWORD PTR [ebp-8], 0		; prime_counter
	mov		ecx,	DWORD PTR [ebp-4]	; loop_counter, -1 when loop.
detect_and_print_prime_iter:
	mov		DWORD PTR [ebp-4],	ecx
	mov		edi,	ecx
	call	is_prime
	cmp		eax,	0
	je		not_prime		; not prime, next turn, otherwise print.
	inc		DWORD PTR [ebp-8]
	; _cdecl call, push parameter from right to left
	push	DWORD PTR [ebp-4]	; prime
	push	DWORD PTR [ebp-8]	; prime_counter
    push    OFFSET format		; format string
    call    printf
not_prime:
	mov		ecx,	DWORD PTR [ebp-4]
	cmp		ecx,	2		; stop until 2 not 1.
	je		quit_main		; if counter == 1, quit.
	loop	detect_and_print_prime_iter
quit_main:
	mov		eax,	0
	mov		esp,	ebp
	pop		ebp
    ret
main ENDP


; edi store the value
sqrt PROC
	mov		ecx,	10
iter_sqrt:
	mov		eax,	ecx
	mul		eax
	cmp		eax,	edi
	jle		ok
	loop iter_sqrt
ok:
	mov		eax,	ecx
	ret
sqrt ENDP


; return 0 if not, 1 otherwise. edi >= 2 !!!
is_prime PROC
	push	ebp
	mov		ebp,	esp
	sub		esp,	4			; [ebp-4] stores the return value.
	mov		DWORD PTR [ebp-4], 1; suppose it is prime at first.
	call sqrt
	mov		ecx,	eax			; ecx = sqrt(edi)
	cmp		ecx,	1			; if ecx == 1, that means edi = 2, 3
	je		quit_prime
div_iter:
	mov		ebx,	ecx			; ebx is divisor.

	mov		eax,	edi			; eax is dividend.
	xor		edx,	edx			; (edx, eax) / ... = eax ... edx
	div		ebx					; division is stored in eax, remainder in edx.

	cmp		edx,	0			; if mod is 0
	je		not_prime			; set eax = 0

	cmp		ecx,	2			; must be 2 rather than 1, debug for too long.........
	je		quit_prime			; if edx == 1, quit.
	loop div_iter

not_prime:
	mov		DWORD PTR [ebp-4],	0
quit_prime:
	mov		eax,	DWORD PTR [ebp-4]
	mov		esp,	ebp
	pop		ebp
	ret
is_prime ENDP


END start