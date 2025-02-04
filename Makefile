all:
	nasm -f elf64 out.asm
	ld -s -o out out.o