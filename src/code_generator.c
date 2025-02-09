#include "code_generator.h"

void generate_code(AST *ptr, FILE *f) {
  AST ast = *ptr;
  switch (ast.tag) {
    // da provare
    case TAG_INT: {
      AST_INT data = ast.data.ast_int;
      fprintf(f, "\tmov rax, %d\n", data.number);
      return;
    }
    // TODO: Correggere implementazione float
    // case TAG_FLOAT: {
    //   AST_FLOAT data = ast.data.ast_float;
    //   fprintf(f, "\tmov rax, %f\n", data.number);
    //   return;
    // }
    case TAG_ASSIGN: {
      AST_ASSIGN data = ast.data.ast_assign;
      generate_code(data.value, f);
      fprintf(f, "\tmov [%s], rax\n", data.name);
      return;
    }
    case TAG_ADD: {
      AST_ADD data = ast.data.ast_add;
      generate_code(data.left, f);
      fprintf(f, "\tpush rax\n");  // Save the left value
      generate_code(data.right, f);
      fprintf(f, "\tpop rbx\n");  // Restore the left value
      fprintf(f, "\tadd rax, rbx\n");  // Add the right value
      return;
    }
    case TAG_SUB: {
      AST_SUB data = ast.data.ast_sub;
      generate_code(data.left, f);
      fprintf(f, "\tpush rax\n");  // Save the left value
      generate_code(data.right, f);
      fprintf(f, "\tpop rbx\n");  // Restore the left value
      fprintf(f, "\tsub rax, rbx\n");  // Subtract the right value
      return;
    }
    case TAG_MUL: {
      AST_MUL data = ast.data.ast_mul;
      generate_code(data.left, f);
      fprintf(f, "\tpush rax\n");  // Save the left value
      generate_code(data.right, f);
      fprintf(f, "\tpop rbx\n");  //  Restore the left value
      fprintf(f, "\tmul rbx\n");  // Multiply the right value
      return;
    }
    case TAG_DIV: {
      AST_DIV data = ast.data.ast_div;
      generate_code(data.left, f);
      fprintf(f, "\tpush rax\n");  // Save the left value
      generate_code(data.right, f);
      fprintf(f, "\tpop rbx\n");  // Restore the left value
      fprintf(f, "\txor rdx, rdx\n");  // Clear rdx
      fprintf(f, "\tdiv rbx\n");  // Divide the right value
      return;
    }
  }
}

void declare_variables(Variable symbol_table[], int size, FILE* f) {
  // inizialized variables
  fprintf(f, "section .data\n");
  
  for (int i = 0; i < size; i++) {
    switch (symbol_table[i].type) {
      case CHAR:
        fprintf(f, "\t%s db %d\n", symbol_table[i].name, symbol_table[i].value.value.cval);  // 8 bit (define byte)
        break;
      case INT:
        fprintf(f, "\t%s dq %ld\n", symbol_table[i].name, symbol_table[i].value.value.i64val);  // 64 bit (define quadword)
        break;
      case FLOAT:
        fprintf(f, "\t%s dd %f\n", symbol_table[i].name, symbol_table[i].value.value.fval);  // 32 bit float (define double word)
        break;
      case STRING:
        // message db 'Ciao, mondo!', 0
        fprintf(f, "\t%s db '%s', 0\n", symbol_table[i].name, symbol_table[i].value.value.sval);  // string
        break;
      default:
        break;
    }
  }

  // not inizialized variables
  fprintf(f, "section .bss\n");
  // TODO: implement this section
}

void generate_asm(AST *ptr, Variable symbol_table[], int size, FILE* f) {
  declare_variables(symbol_table, size, f);
  fprintf(f, "section .text\n");
  fprintf(f, "\tglobal _start\n\n");
  fprintf(f, "_start:\n");

  generate_code(ptr, f);
  fprintf(f, "\n");

  // Exit the program
  fprintf(f, "exit:\n");
  fprintf(f, "\tmov rax, 60\n");   // Exit syscall
  fprintf(f, "\txor rdi, rdi\n");  // Default exit code 0
  fprintf(f, "\tsyscall\n");
}