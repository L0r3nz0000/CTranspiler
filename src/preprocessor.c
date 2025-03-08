#include "preprocessor.h"

void preprocess_code(char* code) {
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