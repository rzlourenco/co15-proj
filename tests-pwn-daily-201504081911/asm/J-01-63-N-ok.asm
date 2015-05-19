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
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL L1
L1:
; STR (value omitted -- see below)
	db	"ola", 0
; TEXT
segment	.text
; ALIGN
align	4
; ADDR L1
	push	dword $L1
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
; LABEL L0
L0:
; LEAVE
	leave
; RET
