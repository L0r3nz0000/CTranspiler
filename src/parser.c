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

AST *new_ast_int64(int64_t number) {
  return new_ast((AST) {
    .tag = TAG_INT,
    .data = {
      .ast_int = {
        .number = number,
      },
    },
  });
}

AST *new_ast_float(float number) {
  return new_ast((AST) {
    .tag = TAG_FLOAT,
    .data = {
      .ast_float = {
        .number = number,
      },
    },
  });
}

AST *new_ast_string(char *string) {
  return new_ast((AST) {
    .tag = TAG_STRING,
    .data = {
      .ast_string = {
        .string = string,
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

// AST *new_ast_declare(VarType type, char *name) {
//   return new_ast((AST) {
//     .tag = TAG_DECLARE,
//     .data = {
//       .ast_declare = {
//         .type = type,
//         .name = name,
//       },
//     },
//   });
// }

/*
Ordine in cui vanno processati i token:
  - moltiplicazioni / divisioni
  - addizioni / sottrazioni
  - operatore di assegnazione
*/

AST *generate_tree(TokenList tl) {
  Variable *symbol_table = NULL;
  if (tl.size == 0) return NULL;

  AST *left, *right;  // Dichiarazioni per eventuali operatori binari
  
  for (int i = 0; i < tl.size; i++) {  // Operatore =
    Token *token = &tl.tokens[i];

    if (token->type == TOKEN_ASSIGN) {
      TokenList sublist = { .tokens = &tl.tokens[i + 1], .size = tl.size - (i + 1) };
      return new_ast_assign(tl.tokens[i-1].value.value.sval, generate_tree(sublist));
    }
  }

  for (int i = 0; i < tl.size; i++) {  // Operatori {+, -}
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_SUM:
        left = generate_tree((TokenList) {tl.tokens, i}); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}); // Token dopo l'operatore
        return new_ast_add(left, right);
      break;
      case TOKEN_SUB:
        left = generate_tree((TokenList) {tl.tokens, i}); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}); // Token dopo l'operatore
        return new_ast_sub(left, right);
      break;
    }
  }
  
  for (int i = 0; i < tl.size; i++) {  // Operatori {*, /}
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_MUL:
        left = generate_tree((TokenList) {tl.tokens, i}); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}); // Token dopo l'operatore
        return new_ast_mul(left, right);
      break;
      case TOKEN_DIV:
        left = generate_tree((TokenList) {tl.tokens, i}); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}); // Token dopo l'operatore
        return new_ast_div(left, right);
      break;
    }
  }

  for (int i = 0; i < tl.size; i++) {  // Casi base (dichiarazioni, nomi variabili, numeri costanti)
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_IDENTIFIER: {
        char *identifier = token->value.value.sval;
        // Per adesso ogni identificatore è considerato come nome di variabile perchè non ci sono ancora operatori
        return new_ast_string(identifier);
      }
      break;
      case TOKEN_NUMBER:
        return new_ast_int64(token->value.value.i32val);
      break;
    }
  }

  return NULL;
}



void print_tree(AST *node, int indent) {
  if (node == NULL) {
    return;
  }

  // Stampa l'indentazione
  for (int i = 0; i < indent; i++) {
    printf("  ");
  }

  // Stampa il nodo in base al tipo
  switch (node->tag) {
    case TAG_ASSIGN:
      printf("Assign: %s\n", node->data.ast_assign.name);
      print_tree(node->data.ast_assign.value, indent + 1);
      break;
    case TAG_INT:
      printf("Int: %ld\n", node->data.ast_int.number);
      break;
    case TAG_FLOAT:
      printf("Float: %f\n", node->data.ast_float.number);
      break;
    case TAG_STRING:
      printf("String: \"%s\"\n", node->data.ast_string.string);
      break;
    case TAG_ADD:
      printf("Add\n");
      print_tree(node->data.ast_add.left, indent + 1);
      print_tree(node->data.ast_add.right, indent + 1);
      break;
    case TAG_SUB:
      printf("Subtract\n");
      print_tree(node->data.ast_sub.left, indent + 1);
      print_tree(node->data.ast_sub.right, indent + 1);
      break;
    case TAG_MUL:
      printf("Multiply\n");
      print_tree(node->data.ast_mul.left, indent + 1);
      print_tree(node->data.ast_mul.right, indent + 1);
      break;
    case TAG_DIV:
      printf("Divide\n");
      print_tree(node->data.ast_div.left, indent + 1);
      print_tree(node->data.ast_div.right, indent + 1);
      break;
    default:
      printf("Unknown node\n");
      break;
  }
}
