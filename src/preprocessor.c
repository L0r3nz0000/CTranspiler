#include "preprocessor.h"

<<<<<<< HEAD
void proprocess(char *code) {
=======
void preprocess_code(char* code) {
>>>>>>> a4bfaf8bbdddff109999d0ac132592ad77d19bc8
  // Remove comments
  for (int i = 0; code[i] != '\0'; i++) {
    if (code[i] == '#') {
      while (code[i] != '\n') {
        code[i] = ' ';
        i++;
      }
    }
  }
<<<<<<< HEAD
}
=======
}
>>>>>>> a4bfaf8bbdddff109999d0ac132592ad77d19bc8
