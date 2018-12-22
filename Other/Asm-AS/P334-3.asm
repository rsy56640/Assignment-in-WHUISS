; P334-3(1)
begin:
	mov		al,		0
up:
	out		220H,	al
	inc		al
	cmp		al,		101H
	jnz		up
	mov		al,		100H ; equals to "dec	al"
down:
	out		220H,	al
	dec		al,		1
	jc		begin		 ;  0 - 1 set the carry flag
	jmp		down


; P334-3(2)
begin:
	mov		al,		3FH
up:
	out		220H,	al
	inc		al
	cmp		al,		D3H
	jnz		up
	call	DELAY_20MS
	mov		al,		D2H ; equals to "dec	al"
down:
	out		220H,	al
	dec		al,		1
	cmp		al,		3EH
	jne		down
	jmp		begin