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
; ENTER 16
	push	ebp
	mov	ebp, esp
	sub	esp, 16
; LOCAL -8
	lea	eax, [ebp+-8]
	push	eax
; DLOAD
	pop	eax
	push	dword [eax+4]
	push	dword [eax]
; TEXT
segment	.text
; ALIGN
align	4
; EXTERN printi
extern	printi
; CALL printi
	call	printi
; TRASH 4
	add	esp, 4
; LABEL L0
L0:
; LEAVE
	leave
; RET
