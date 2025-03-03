section .bss
	var1 resq 1
	var2 resq 1
section .data
	int_fmt db "%d", 0
	float_fmt db "%f", 0

section .text
	global _start

	global print_int
	global print_float
	extern printf

print_int:
    push rbp
    mov rbp, rsp
    mov rsi, rdi     ; Sposta il numero in rsi (secondo argomento di printf) 
    lea rdi, [int_fmt]   ; Carica l'indirizzo della stringa di formato in rdi 
    xor eax, eax     ; Puliamo rax per la chiamata variadica di printf 
    call printf      ; Chiama printf 
    pop rbp
    ret
print_float:
    push rbp
    mov rbp, rsp
    mov rsi, rdi     ; Sposta il numero in rsi (secondo argomento di printf) 
    lea rdi, [float_fmt]   ; Carica l'indirizzo della stringa di formato in rdi 
    xor eax, eax     ; Puliamo rax per la chiamata variadica di printf 
    call printf      ; Chiama printf 
    pop rbp
    ret
_start:
	call main

	xor rdi, 0
	mov rax, 60
	syscall
somma:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	mov QWORD [rbp - 8], rdi
	mov QWORD [rbp - 16], rsi
	mov rsp, rbp
	pop rbp
	ret
quadrato:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	mov QWORD [rbp - 8], rdi
	mov rsp, rbp
	pop rbp
	ret
main:
	push rbp
	mov rbp, rsp
	mov rax, 3
	push rax
	mov rax, 2
	pop rbx
	imul rax, rbx
	push rax
	mov rax, 4
	pop rbx
	add rax, rbx
	mov [rbp - 8], rax
	push rax
	mov rax, 3
	pop rbx
	sub rax, rbx
	mov [rbp - 16], rax
	mov rdi, rax
	call somma
	mov rdi, rax
	mov rsi, rax
	call print_int
	mov rdi, rax
	call quadrato
	mov rdi, rax
	mov rsi, rax
	call print_int
	mov rsp, rbp
	pop rbp
	ret
