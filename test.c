#include "src/lexer.h"
#include "src/parser.h"

int main() {
  char *code = "@variabile = 3;";
  TokenList tokens = tokenize_code(code);
  print_token_list(tokens);
  
  return 0;
}