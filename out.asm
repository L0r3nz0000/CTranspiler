section .data
	a dd 1
	b dd 2
	c dd 3
section .bss
section .text
	global _start

_start:
	mov rax, 1
	push rax
	mov rax, 2
	pop rbx
	add rax, rbx
	mov [a], rax

exit:
	mov rax, 60
	xor rdi, rdi
	syscall
