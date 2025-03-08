#include "preprocessor.h"

void proprocess(char *code) {
  // Remove comments
  for (int i = 0; code[i] != '\0'; i++) {
    if (code[i] == '#') {
      while (code[i] != '\n') {
        code[i] = ' ';
        i++;
      }
    }
  }
}
