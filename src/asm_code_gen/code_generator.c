#include "code_generator.h"

char *calling_conv_registers[] = { "rdi", "rsi", "rdx", "rcx", "r8", "r9" };

SYMBOL_TABLE *symbol_table = NULL;

void generate_code(AST_BLOCK *ptr, FILE *f);

void declare_variables(FILE *f, SYMBOL_TABLE *symbol_table) {
  fprintf(f, "section .bss\n");

  for (int i = 0; i < symbol_table->count; i++) {
    switch (symbol_table->vars[i].type) {
      case INT:
        fprintf(f, "\t%s resq 1\n", symbol_table->vars[i].name);  // QWORD (8 byte) per int 64-bit
        break;
      case CHAR:
        fprintf(f, "\t%s resb 1\n", symbol_table->vars[i].name);  // BYTE (1 byte) per char
        break;
      case FLOAT:
        fprintf(f, "\t%s resd 1\n", symbol_table->vars[i].name); // QWORD (8 byte) per float 64-bit
        break;
    }
  }

  fprintf(f, "section .data\n");
  fprintf(f, "\tint_fmt db \"%%d\", 0\n");  // Stringa di formato per int
  fprintf(f, "\tfloat_fmt db \"%%f\", 0\n\n");  // Stringa di formato per float
}

void define_function(AST *ast, FILE *f) {
  AST_FUNCT func = ast->data.ast_funct;
  fprintf(f, "%s:\n", func.name);
  printf("Definisco la funzione: %s\n", func.name);

  // Prologo della funzione
  fprintf(f, "\tpush rbp\n");       // Salva il vecchio base pointer
  fprintf(f, "\tmov rbp, rsp\n");   // Imposta il nuovo base pointer

  // Riserviamo spazio per variabili locali
  int local_var_size = func.param_count * 8;  // Supponiamo variabili a 64-bit
  if (local_var_size > 0) {
    fprintf(f, "\tsub rsp, %d\n", local_var_size);
  }

  // Assegnazione dei parametri alle variabili locali
  for (int i = 0; i < func.param_count; i++) {
    if (i < 6) {
      // Parametri passati nei registri
      fprintf(f, "\tmov QWORD [rbp - %d], %s\n", (i + 1) * 8, calling_conv_registers[i]);
    } else {
      // Parametri passati nello stack (oltre i primi 6)
      int stack_offset = 16 + ((i - 6) * 8);
      fprintf(f, "\tmov rax, [rbp + %d]\n", stack_offset);
      fprintf(f, "\tmov QWORD [rbp - %d], rax\n", (i + 1) * 8);
    }
  }

  generate_code(func.body, f);

  // Epilogo della funzione
  fprintf(f, "\tmov rsp, rbp\n");
  fprintf(f, "\tpop rbp\n");
  fprintf(f, "\tret\n");
}

int get_var_offset(const char *var_name) {
  // Scorri la tabella dei simboli per trovare la variabile
  for (int i = 0; i < symbol_table->count; i++) {
    if (strcmp(symbol_table->vars[i].name, var_name) == 0) {
      // Offset della variabile (distanza da RBP)
      return ((i + 1) * 8);
    }
  }

  // Se la variabile non è trovata, errore
  fprintf(stderr, "Errore: Variabile %s non trovata!\n", var_name);
  exit(1);
}

void generate_code(AST_BLOCK *ptr, FILE *f) {
  for (int i = 0; i < ptr->count; i++) {
    AST ast = *ptr->statements[i];

    switch (ast.tag) {
      case TAG_VAR: {
        AST_VAR data = ast.data.ast_var;
        int offset = get_var_offset(data.name);
        fprintf(f, "\tmov [rbp - %d], rax\n", offset); // Salva la variabile locale
        continue;
      }
      case TAG_INT: {
        AST_INT data = ast.data.ast_int;
        fprintf(f, "\tmov rax, %d\n", data.number);
        continue;
      }
      case TAG_FUN: {
        define_function(ptr->statements[i], f);
        continue;
      }
      case TAG_CALL: {
        AST_CALL data = ast.data.ast_call;
        
        // Spazio per eventuali argomenti extra da passare nello stack
        int stack_args = (data.arg_count > 6) ? (data.arg_count - 6) : 0;
        
        // Allineare lo stack a 16 byte prima della chiamata (se necessario)
        if ((stack_args * 8) % 16 != 0) {
          fprintf(f, "\tsub rsp, 8\n");  // Padding per mantenere l'allineamento
        }

        // Passa i parametri in ordine inverso nello stack (oltre i primi 6)
        for (int i = data.arg_count - 1; i >= 6; i--) {
          generate_code(&(AST_BLOCK){&data.args[i], 1}, f);
          fprintf(f, "\tpush rax\n");
        }

        // Passa i primi 6 parametri nei registri appropriati
        for (int i = 0; i < data.arg_count && i < 6; i++) {
          generate_code(&(AST_BLOCK){&data.args[i], 1}, f);
          fprintf(f, "\tmov %s, rax\n", calling_conv_registers[i]);
        }

        // Chiamata alla funzione
        fprintf(f, "\tcall %s\n", data.name);

        // Pulizia dello stack se ci sono parametri extra
        if (stack_args > 0) {
          fprintf(f, "\tadd rsp, %d\n", stack_args * 8);
        }

        // Ripristino dell'allineamento dello stack
        if ((stack_args * 8) % 16 != 0) {
          fprintf(f, "\tadd rsp, 8\n");
        }
        continue;
      }

      // case TAG_RETURN: {
      //   AST_RETURN data = ast.data.ast_return;
      //   generate_code(&(AST_BLOCK){&data.value, 1}, f);
      //   fprintf(f, "\tret\n");
      //   continue;
      // }
      case TAG_ASSIGN: {
        AST_ASSIGN data = ast.data.ast_assign;
        generate_code(&(AST_BLOCK){&data.value, 1}, f);
        int offset = get_var_offset(data.name);
        fprintf(f, "\tmov [rbp - %d], rax\n", offset);
        continue;
      }
      case TAG_ADD: {
        AST_ADD data = ast.data.ast_add;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, "\tpush rax\n");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        fprintf(f, "\tpop rbx\n");
        fprintf(f, "\tadd rax, rbx\n");
        continue;
      }
      case TAG_SUB: {
        AST_SUB data = ast.data.ast_sub;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, "\tpush rax\n");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        fprintf(f, "\tpop rbx\n");
        fprintf(f, "\tsub rax, rbx\n");
        continue;
      }
      case TAG_MUL: {
        AST_MUL data = ast.data.ast_mul;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, "\tpush rax\n");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        fprintf(f, "\tpop rbx\n");
        fprintf(f, "\timul rax, rbx\n");  // Corretto per x86-64
        continue;
      }
      case TAG_DIV: {
        AST_DIV data = ast.data.ast_div;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, "\tpush rax\n");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        fprintf(f, "\tpop rbx\n");
        fprintf(f, "\tcqto\n");   // Sign-extend rax in rdx
        fprintf(f, "\tidiv rbx\n");
        continue;
      }
    }
  }
}

void include_stdlib(FILE *f) {
  fprintf(f, "\tglobal print_int\n");
  fprintf(f, "\tglobal print_float\n");
  fprintf(f, "\textern printf\n\n");
  fprintf(f, """print_int:\n\
    push rbp\n\
    mov rbp, rsp\n\
    mov rsi, rdi     ; Sposta il numero in rsi (secondo argomento di printf) \n\
    lea rdi, [int_fmt]   ; Carica l'indirizzo della stringa di formato in rdi \n\
    xor eax, eax     ; Puliamo rax per la chiamata variadica di printf \n\
    call printf      ; Chiama printf \n\
    pop rbp\n\
    ret\n""");

  fprintf(f, """print_float:\n\
    push rbp\n\
    mov rbp, rsp\n\
    mov rsi, rdi     ; Sposta il numero in rsi (secondo argomento di printf) \n\
    lea rdi, [float_fmt]   ; Carica l'indirizzo della stringa di formato in rdi \n\
    xor eax, eax     ; Puliamo rax per la chiamata variadica di printf \n\
    call printf      ; Chiama printf \n\
    pop rbp\n\
    ret\n""");
}

void generate_asm(PROGRAM *program, FILE* f, bool no_stdlib) {
  int size = 2;
  symbol_table = program->symbol_table;

  declare_variables(f, program->symbol_table);
  fprintf(f, "section .text\n");
  fprintf(f, "\tglobal _start\n\n");

  if (!no_stdlib)
    include_stdlib(f);

  fprintf(f, "_start:\n");
  fprintf(f, "\tcall main\n\n");

  // Exit
  fprintf(f, "\txor rdi, 0\n");  // Default exit code 0
  fprintf(f, "\tmov rax, 60\n");   // Exit syscall
  fprintf(f, "\tsyscall\n");

  // Genera l'assembly per tutti i sottoalberi del blocco di codice
  generate_code(program->block, f);
  
}