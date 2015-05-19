; TEXT
segment	.text
; ALIGN
align	4
; TEXT
segment	.text
; ALIGN
align	4
; LABEL f
f:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; INT 0
	push	dword 0
; EQ
	pop	eax
	xor	ecx, ecx
	cmp	[esp], eax
	sete	cl
	mov	[esp], ecx
; JZ L1
	pop	eax
	cmp	eax, byte 0
	je	near L1
; INT 0
	push	dword 0
; DDUP
; SP
	push	esp
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; JMP L2
	jmp	dword L2
; LABEL L1
L1:
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; INT 1
	push	dword 1
; SUB
	pop	eax
	sub	dword [esp], eax
; CALL f
	call	f
; TRASH 8
	add	esp, 8
; DPUSH
	sub	esp, byte 8
	fstp	qword [esp]
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; ADD
	pop	eax
	add	dword [esp], eax
; DDUP
; SP
	push	esp
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; JMP L0
	jmp	dword L0
; LABEL L2
L2:
; INT 9
	push	dword 9
; DDUP
; SP
	push	esp
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DSTORE
	pop	ecx
	pop	eax
	mov	[ecx], eax
	pop	eax
	mov	[ecx+4], eax
; TRASH 8
	add	esp, 8
; LABEL L0
L0:
; DPOP
	fld	qword [esp]
	add	esp, byte 8
; LEAVE
	leave
; RET
