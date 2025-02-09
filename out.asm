section .data
	variabile1 dq 1
section .bss
section .text
	global _start

_start:
	push rax
	mov rax, 3
	pop rbx
	sub rax, rbx
	mov [variabile2], rax
	mov [variabile1], rax

exit:
	mov rax, 60
	xor rdi, rdi
	syscall
