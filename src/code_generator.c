#include "code_generator.h"

char *calling_conv_registers[] = {
  "RDI", "RSI", "RDX", "RCX", "R8", "R9"
};

void generate_code(AST *ptr, FILE *f);

void define_function(AST *ast, FILE *f) {
  AST_FUNCT func = ast->data.ast_funct;
  fprintf(f, "%s:\n", func.name);

  // Prologo della funzione
  fprintf(f, "\tpush rbp\n");
  fprintf(f, "\tmov rbp, rsp\n");

  // Assegnazione dei parametri ai nomi delle variabili
  for (int i = 0; i < func.param_count; i++) {
    fprintf(f, "\tmov [rbp - %d], %s\n", (i + 1) * 8, calling_conv_registers[i]);
  }

  for (int i = 0; i < func.body->count; i++) {
    generate_code(func.body->statements[i], f);
  }

  fprintf(f, "\tmov rsp, rbp\n");
  fprintf(f, "\tpop rbp\n");
  fprintf(f, "\tret\n");
}

// sum_numbers:
//   ; Parametri:
//   ; RDI = primo numero
//   ; RSI = secondo numero

//   add rdi, rsi   ; Somma i due numeri
//   mov rax, rdi   ; Il risultato va in RAX (valore di ritorno)
//   ret            ; Ritorna al chiamante


void generate_code(AST *ptr, FILE *f) {
  AST ast = *ptr;
  switch (ast.tag) {
    case TAG_VAR: {
      AST_VAR data = ast.data.ast_var;
      fprintf(f, "\tmov [%s], rax\n", data.name);
      return;
    }
    case TAG_INT: {
      AST_INT data = ast.data.ast_int;
      fprintf(f, "\tmov rax, %d\n", data.number);
      return;
    }
    case TAG_FUN: {
      define_function(ptr, f);
      return;
    }

    case TAG_RETURN: {
      AST_RETURN data = ast.data.ast_return;
      generate_code(data.value, f);  // Genera l'espressione
      fprintf(f, "\tmov rax, rax\n");  // Assicura che il valore sia in RAX
      fprintf(f, "\tret\n");  // Ritorna alla funzione chiamante
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
      fprintf(f, "\tmov rdx, 0\n");  // Clear rdx 
      fprintf(f, "\tidiv rbx\n");  // Divisione signed
      return;
    }
  }
}

void declare_variables(Variable symbol_table[], int size, FILE* f) {
  // inizialized variables
  fprintf(f, "section .data\n");

  fprintf(f, "\tint_format db \"%%d\", 10, 0\n");
  
  for (int i = 0; i < size; i++) {
    if (!symbol_table[i].value.is_none) {
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
  }

  // not inizialized variables
  fprintf(f, "section .bss\n");
  // TODO: implement this section
}

void generate_asm(AST_BLOCK *block, FILE* f) {
  Variable symbol_table[] = {
    {"variabile1", INT, {.is_none = 0}},
    {"variabile2", INT, {.is_none = 0}},
  };
  int size = 2;

  declare_variables(symbol_table, size, f);
  fprintf(f, "section .text\n");
  fprintf(f, "\tglobal _start\n\n");
  fprintf(f, "\textern printf\n\n");
  fprintf(f, "_start:\n");

  for (int i = 0; i < block->count; i++) {  // Genera l'assembly per tutti i sottoalberi del blocco di codice
    generate_code(block->statements[i], f);
  }

  fprintf(f, "\n");

  // Exit the program
  fprintf(f, "exit:\n");
  fprintf(f, "\tmov rax, 60\n");   // Exit syscall
  fprintf(f, "\txor rdi, rdi\n");  // Default exit code 0
  fprintf(f, "\tsyscall\n");
}