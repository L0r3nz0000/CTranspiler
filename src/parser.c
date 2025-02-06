#include "parser.h"

AST *new_ast(AST ast) {
  AST *ptr = malloc(sizeof(AST));

  if (ptr != NULL) {
    *ptr = ast;
  }
  return ptr;
}

AST *new_ast_assign(char *name, AST *value) {
  return new_ast((AST) {
    .tag = TAG_ASSIGN,
    .data = {
      .ast_assign = {
        .name = name,
        .value = value,
      },
    },
  });
}

AST *new_ast_number(int number) {
  return new_ast((AST) {
    .tag = TAG_NUMBER,
    .data = {
      .ast_number = {
        .number = number,
      },
    },
  });
}

AST *new_ast_add(AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_ADD,
    .data = {
      .ast_add = {
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_sub(AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_SUB,
    .data = {
      .ast_sub = {
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_mul(AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_MUL,
    .data = {
      .ast_mul = {
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_div(AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_DIV,
    .data = {
      .ast_div = {
        .left = left,
        .right = right,
      },
    },
  });
}

/*
Scorre in modo lineare più volte cercando le operazioni in ordine di priorità
e le aggiunge all'albero della sintassi astratta
*/

AST *analyze_tokens(TokenList tl) {
  Variable *symbol_table = NULL;
  AST *ast = NULL;

  for (int i = 0; i < tl.size; i++) {
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case 
    }
  }
}