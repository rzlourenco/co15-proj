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
; INT 2
	push	dword 2
; LOCA -8
	pop	eax
	mov	[ebp-8], eax
; LABEL L0
L0:
; DPOP
	fld	qword [esp]
	add	esp, byte 8
; LEAVE
	leave
; RET
