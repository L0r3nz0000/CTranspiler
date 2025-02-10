section .data
	int_format db "%d", 10, 0
	variabile1 dq 0
	variabile2 dq 0
section .bss
section .text
	global _start

	extern printf

_start:
	mov rax, 3
	push rax
	mov rax, 2
	pop rbx
	mul rbx
	push rax
	mov rax, 4
	pop rbx
	add rax, rbx
	mov [variabile1], rax
	push rax
	mov rax, 3
	pop rbx
	sub rax, rbx
	mov [variabile2], rax
mov rsi, rax
lea rdi, [int_format]
xor rax, rax
call printf
mov rsi, rax
lea rdi, [int_format]
xor rax, rax
call printf
mov rsi, rax
lea rdi, [int_format]
xor rax, rax
call printf

exit:
	mov rax, 60
	xor rdi, rdi
	syscall
