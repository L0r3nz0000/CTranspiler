#include "src/lexer.h"

int main() {
  char *code = "@variabile = 3;";
  TokenList tokens = analyze_code(code);

  print_token_list(tokens);

  return 0;
}