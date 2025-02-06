#include <stdio.h>  // FILE, printf
#include "parser.h"  // AST, Variable
#include "lexer.h"   // Token, TokenList, analyze_code(), print_token_list()
#include "code_generator.h"  // generate_asm()

char* read_code_from_file(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening file");
    return NULL;
  }

  // Place the cursor at the end of the file
  fseek(file, 0, SEEK_END);
  // Get the file size
  long fileSize = ftell(file);
  if (fileSize == -1) {
    fprintf(stderr, "Error determining file size");
    fclose(file);
    return NULL;
  }
  // Go back to the beginning of the file
  fseek(file, 0, SEEK_SET);

  // Allocate memory for the string
  char* content = (char*)malloc(fileSize + 1);
  if (content == NULL) {
    fprintf(stderr, "Memory allocation error");
    fclose(file);
    return NULL;
  }

  // Read file content
  size_t letti = fread(content, 1, fileSize, file);
  if (letti != fileSize) {
    fprintf(stderr, "Error reading file");
    free(content);
    fclose(file);
    return NULL;
  }

  // Add terminating character
  content[fileSize] = '\0';

  // Close file
  fclose(file);

  return content;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [source.l]\n", argv[0]);
    exit(1);
  }

  char *code = read_code_from_file(argv[1]);

  printf("loading code: \n\n%s\n\n", code);

  if (code == NULL) {
    exit(1);
  }

  // Tokenize the code
  TokenList tokens = analyze_code(code);

  print_token_list(tokens);

  Variable symbol_table[] = {
    {"a", INT32, {1}},
    {"b", INT32, {2}},
    {"c", INT32, {3}},
  };
  int size = 3;

  // Test AST: a = 1 + 2 * 3

  AST *n1 = new_ast_number(2);
  AST *n2 = new_ast_number(3);
  AST *mul = new_ast_mul(n1, n2);

  AST *n1 = new_ast_number(1);
  AST *sum = new_ast_sum(n1, mul);

  AST *ast = new_ast_assign("a", sum);

  FILE *f = fopen("out.asm", "w");
  if (f == NULL) {
    printf("Error opening assembly file!\n");
    exit(1);
  }
  
  // Write assembly on file
  generate_asm(ast, symbol_table, size, f);

  // Save assembly file
  fclose(f);

  // Compiles the assembly with nasm
  system("nasm -f elf64 -g out.asm -o out.o");
  system("ld -g -o out out.o");  // Links the binary
}