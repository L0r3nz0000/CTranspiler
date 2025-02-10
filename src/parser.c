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

AST *new_ast_funct(char *name, char **params, int param_count, AST_BLOCK *body) {
  return new_ast((AST) {
    .tag = TAG_FUN,
    .data = {
      .ast_funct = {
        .name = name,
        .params = params,
        .param_count = param_count,
        .body = body,
      },
    },
  });
}

AST *new_ast_call(char *name, AST **args, int arg_count) {
  return new_ast((AST) {
    .tag = TAG_CALL,
    .data = {
      .ast_call = {
        .name = name,
        .args = args,
        .arg_count = arg_count,
      },
    },
  });
}

AST *new_ast_return(AST *value) {
  return new_ast((AST) {
    .tag = TAG_RETURN,
    .data = {
      .ast_return = {
        .value = value,
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

AST *generate_tree(TokenList tl) {
  Variable *symbol_table = NULL;
  if (tl.size == 0 || tl.tokens == NULL) { return NULL; }


  AST *left, *right;  // Dichiarazioni per eventuali operatori binari

  if (tl.tokens[0].type == TOKEN_RETURN) {
    // Considera i token successivi come espressione di ritorno
    TokenList return_expr = { .tokens = &tl.tokens[1], .size = tl.size - 1 };
    AST *value = generate_tree(return_expr);  // Genera l'espressione che viene restituita
    return new_ast_return(value);  // Crea il nodo return
  }

  // Controlla se è una dichiarazione di funzione
  if (tl.tokens[0].type == TOKEN_FUN) {
    char *name = tl.tokens[1].value.value.sval; // Nome funzione
    int param_count = 0;
    char **params = malloc(sizeof(char *) * 6); // Max 6 parametri

    // Legge i parametri
    int i = 3;  // Salta "fun", il nome e (
    while (tl.tokens[i].type != TOKEN_RPAREN) {
      if (tl.tokens[i].type == TOKEN_IDENTIFIER) {
        params[param_count++] = tl.tokens[i].value.value.sval;
      }
      i++;
    }

    i++; // Salta il token =>

    // Analizza il corpo della funzione (dopo '{')
    TokenList sublist = { .tokens = &tl.tokens[i + 2], .size = tl.size - (i + 3) };
    //AST *body = generate_tree(sublist);
    AST_BLOCK *block = parse_program(sublist);

    // Crea il nodo funzione
    return new_ast_funct(name, params, param_count, block);
  }

  // Controlla se è una chiamata di funziona
  for (int i = 0; i < tl.size; i++) {
    if (tl.tokens[i].type == TOKEN_IDENTIFIER && tl.tokens[i + 1].type == TOKEN_LPAREN) {
      char *name = tl.tokens[i].value.value.sval;
      int arg_count = 0;
      AST **args = malloc(sizeof(AST *) * 6);

      int j = i + 2;
      while (tl.tokens[j].type != TOKEN_RPAREN) {
        args[arg_count++] = generate_tree((TokenList){.tokens = &tl.tokens[j], .size = 1});
        j += 2; // Salta le virgole
      }

      return new_ast_call(name, args, arg_count);
    }
  }

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

AST_BLOCK *parse_program(TokenList tl) {
  if (tl.size == 0) return NULL;

  AST_BLOCK *block = malloc(sizeof(AST_BLOCK));
  block->statements = malloc(tl.size * sizeof(AST *));
  block->count = 0;

  int start = 0;
  for (int i = 0; i < tl.size; i++) {
    if (tl.tokens[i].type == TOKEN_EOL) { // New line
      TokenList sublist = { .tokens = &tl.tokens[start], .size = i - start };
      block->statements[block->count++] = generate_tree(sublist);
      start = i + 1;
    }
  }

  // Se c'è ancora un'espressione non processata (ultima riga)
  if (start < tl.size) {
    TokenList sublist = { .tokens = &tl.tokens[start], .size = tl.size - start };
    block->statements[block->count++] = generate_tree(sublist);
  }

  return block;
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
