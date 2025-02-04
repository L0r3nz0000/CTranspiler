#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *f = fopen("out.asm", "w");
  if (f == NULL) {
    printf("Error writing assembly file!\n");
    exit(1);
  }
  fprintf(f, "section .data\n");
  fprintf(f, "section .bss\n");
  fprintf(f, "section .text\n");
  fprintf(f, "global _start\n");
  fprintf(f, "_start:\n");
  fprintf(f, "mov eax, 1\n");
  fprintf(f, "mov ebx, 42\n");
  fprintf(f, "int 0x80\n");
  fprintf(f, "mov eax, 1\n");
  fprintf(f, "mov ebx, 0\n");
  fprintf(f, "int 0x80\n");
  fclose(f);
}