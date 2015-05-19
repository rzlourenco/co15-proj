; TEXT
segment	.text
; ALIGN
align	4
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
; INT 10
	push	dword 10
; LOCA -8
	pop	eax
	mov	[ebp-8], eax
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; CALL g
	call	g
; TRASH 8
	add	esp, 8
; DPUSH
	sub	esp, byte 8
	fstp	qword [esp]
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
	ret
; TEXT
segment	.text
; ALIGN
align	4
; LABEL g
g:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; INT 14
	push	dword 14
; LOCA -8
	pop	eax
	mov	[ebp-8], eax
; LOCAL 8
	lea	eax, [ebp+8]
	push	eax
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; TEXT
segment	.text
; ALIGN
align	4
; EXTERN prints
extern	prints
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; LABEL L1
L1:
; DPOP
	fld	qword [esp]
	add	esp, byte 8
; LEAVE
	leave
; RET
