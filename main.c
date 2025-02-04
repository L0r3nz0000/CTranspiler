#include <stdio.h>
#include <stdlib.h>

typedef union {
  char cval;
  long i64val;   // int 64
  int i32val;    // int 32
  short i16val;  // int 16
  float fval;
  char *sval;
} Value;

typedef enum {
  CHAR,
  INT16,
  INT32,
  INT64,
  FLOAT,
  STRING,
} Type;

typedef struct {
  char *name;
  Type type;
  Value value;
} Variable;

typedef enum {
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  AND,
  OR,
  NOT,
  GT,
  LT,
  GE,
  LE,
  EQ,
  NE,
  ASSIGN,
  IF,
  WHILE,
  FOR,
  DO,
  BREAK,
  CONTINUE,
  RETURN,
  CALL,
  ARG,
  FUNCTION,
  BLOCK,
  VAR,
  EXPR,
  CONST,
  ID,
} NodeType;

typedef union {

} Operation;

typedef struct {
  int radice;
  SintaxTree *left, *right;
} SintaxTree;

int main() {
  Variable symbol_table[] = {
    {"a", INT32, 1},
    {"b", INT32, 2},
    {"c", INT32, 3},
  };




  FILE *f = fopen("out.asm", "w");
  if (f == NULL) {
    printf("Error writing assembly file!\n");
    exit(1);
  }

  fprintf(f, "section .data\n");
  for () {
    fprintf(f, "%s dq %d\n", 10);  // 64 bit
    fprintf(f, "%s dd %d\n", 10);  // 32 bit
  }

  fprintf(f, "section .bss\n");
  fprintf(f, "section .text\n");
  fprintf(f, "global _start\n");
  fprintf(f, "_start:\n");

  

  // Write the system call to stdout
  fprintf(f, "mov eax, 1\n");
  fprintf(f, "mov ebx, 42\n");
  fprintf(f, "int 0x80\n");


  // Exit the program
  fprintf(f, "mov rax, 60");   // Exit syscall
  fprintf(f, "xor rdi, rdi");  // Default exit code 0
  fprintf(f, "syscall");

  fclose(f);
}