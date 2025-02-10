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
  // if (argc != 2) {
  //   fprintf(stderr, "Usage: %s [source.l]\n", argv[0]);
  //   exit(1);
  // }
  argv[1] = "src.l";

  char *code = read_code_from_file(argv[1]);

  printf("loading code: \n\n%s\n\n", code);

  if (code == NULL) {
    exit(1);
  }

  // Tokenize the code
  TokenList tokens = analyze_code(code);

  print_token_list(tokens);
  printf("\n");

  AST_BLOCK *program = parse_program(tokens);

  for (int i = 0; i < program->count; i++) {
    print_tree(program->statements[i], 0);
  }

  FILE *f = fopen("out.asm", "w");
  if (f == NULL) {
    printf("Error opening assembly file!\n");
    exit(1);
  }
  
  // Write assembly on file
  generate_asm(program, f);

  // Save assembly file
  fclose(f);

  // Compiles the assembly with nasm
  system("nasm -f elf64 -g out.asm -o out.o");
  system("gcc -no-pie out.o -o out -nostartfiles");  // Links the binary
}