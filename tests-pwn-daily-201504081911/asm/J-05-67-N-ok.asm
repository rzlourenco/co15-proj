; TEXT
segment	.text
; ALIGN
align	4
; TEXT
segment	.text
; ALIGN
align	4
; LABEL pi
pi:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; INT 314
	push	dword 314
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
