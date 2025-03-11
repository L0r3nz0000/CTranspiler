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

AST *new_ast_add(VarType ret_type, AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_ADD,
    .data = {
      .ast_add = {
        .ret_type = ret_type,
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_sub(VarType ret_type, AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_SUB,
    .data = {
      .ast_sub = {
        .ret_type = ret_type,
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_mul(VarType ret_type, AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_MUL,
    .data = {
      .ast_mul = {
        .ret_type = ret_type,
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_div(VarType ret_type, AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_DIV,
    .data = {
      .ast_div = {
        .ret_type = ret_type,
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_mod(VarType ret_type, AST *left, AST *right) {
  return new_ast((AST) {
    .tag = TAG_MOD,
    .data = {
      .ast_mod = {
        .ret_type = ret_type,
        .left = left,
        .right = right,
      },
    },
  });
}

AST *new_ast_funct(VarType ret_type, char *name, VarType *param_types, char **params, int param_count, AST_BLOCK *body) {
  return new_ast((AST) {
    .tag = TAG_FUN,
    .data = {
      .ast_funct = {
        .ret_type = ret_type,
        .name = name,
        .param_types = param_types,
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


AST *new_ast_declare(VarType type, char *name, AST *value) {
  return new_ast((AST) {
    .tag = TAG_DECLARE,
    .data = {
      .ast_declare = {
        .type = type,
        .name = name,
        .value = value,
      },
    },
  });
}

AST *new_ast_var(VarType type, char *name) {
  return new_ast((AST) {
    .tag = TAG_VAR,
    .data = {
      .ast_var = {
        .type = type,
        .name = name,
      },
    },
  });
}

AST *new_ast_system_import(char *filename) {
  return new_ast((AST) {
    .tag = TAG_SYSTEM_IMPORT,
    .data = {
      .ast_system_import = {
        .filename = filename,
      },
    },
  });
}

AST *new_ast_if(AST *condition, AST_BLOCK *body) {
  return new_ast((AST) {
    .tag = TAG_IF,
    .data = {
      .ast_if = {
        .condition = condition,
        .body = body,
      },
    },
  });
}

AST *new_ast_for(AST *init, AST *condition, AST *step, AST_BLOCK *body) {
  return new_ast((AST) {
    .tag = TAG_FOR,
    .data = {
      .ast_for = {
        .init = init,
        .condition = condition,
        .step = step,
        .body = body,
      },
    },
  });
}

AST *new_ast_while(AST *condition, AST_BLOCK *body) {
  return new_ast((AST) {
    .tag = TAG_WHILE,
    .data = {
      .ast_while = {
        .condition = condition,
        .body = body,
      },
    },
  });
}

AST *new_ast_condition(AST *left, AST *right, TokenType type) {
  return new_ast((AST) {
    .tag = TAG_CONDITION,
    .data = {
      .ast_condition = {
        .left = left,
        .right = right,
        .type = type,
      },
    },
  });
}

VarType typeOf(AST* ast) {
  VarType t = VOID;
  VarType t1, t2;

  switch (ast->tag) {
    case TAG_INT:
      t = INT;
      break;
    case TAG_FLOAT:
      t = FLOAT;
      break;
    case TAG_STRING:
      t = STRING;
      break;
    case TAG_FUN:  // da verificare il funzionamento
      t = FUNCTION;
      break;
    case TAG_VAR:
      t = ast->data.ast_var.type;
      break;
    case TAG_ADD:
      t1 = typeOf(ast->data.ast_add.left);
      t2 = typeOf(ast->data.ast_add.right);

      if (t1 == INT && t2 == INT) {
        t = INT;
      } else {
        t = FLOAT;
      }
      break;
    case TAG_SUB:
      t1 = typeOf(ast->data.ast_sub.left);
      t2 = typeOf(ast->data.ast_sub.right);

      if (t1 == INT && t2 == INT) {
        t = INT;
      } else {
        t = FLOAT;
      }
      break;
    case TAG_MUL:
      t1 = typeOf(ast->data.ast_mul.left);
      t2 = typeOf(ast->data.ast_mul.right);

      if (t1 == INT && t2 == INT) {
        t = INT;
      } else {
        t = FLOAT;
      }
      break;
    case TAG_DIV:
      t = FLOAT;
      break;
    case TAG_DECLARE:
      t = ast->data.ast_declare.type;
      break;
    default:
      printf("Invalid operation (typeof) for tag: %d\n", ast->tag);
      exit(1);
  }

  return t;
}

void add_ast_to_block(AST_BLOCK *block, AST *node) {
  if (!block || !node) return;

  // Se il blocco non ha ancora un array di statements, lo allochiamo
  if (block->statements == NULL) {
    block->statements = malloc(sizeof(AST *));
    if (!block->statements) {
      fprintf(stderr, "Errore: impossibile allocare memoria per AST_BLOCK\n");
      exit(1);
    }
  } else {  
    // Aggiungiamo spazio per il nuovo nodo
    AST **temp = realloc(block->statements, sizeof(AST *) * (block->count + 1));
    if (!temp) {
      fprintf(stderr, "Errore: impossibile allocare memoria per AST_BLOCK\n");
      exit(1);
    }
    block->statements = temp;
  }

  // Aggiungiamo il nodo alla fine
  block->statements[block->count] = node;
  block->count++;
}


size_t find(TokenList tl, TokenType type) {
  size_t index = -1;
  for (int i = 0; i < tl.size; i++) {
    if (tl.tokens[i].type == type) {
      index = i;
      break;
    }
  }
  return index;
}

size_t find_last(TokenList tl, TokenType type) {
  size_t index = -1;
  for (int i = tl.size - 1; i >= 0; i--) {
    if (tl.tokens[i].type == type) {
      index = i;
      break;
    }
  }
  return index;
}

TokenList slice(TokenList tl, int start, int elements) {
  return (TokenList) { .tokens = &tl.tokens[start], .size = elements };
}

TokenList slice_from_to(TokenList tl, int start, int end) {
  return slice(tl, start, end - start);
}

size_t find_next_comma(TokenList tl, size_t j) {
  if (j >= tl.size) return -1; // Evita accessi fuori limite
  TokenList sublist = slice(tl, j + 1, tl.size - (j + 1)); 
  size_t relative_index = find(sublist, TOKEN_COMMA);
  return (relative_index != (size_t)-1) ? j + 1 + relative_index : -1;
}

int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}

TokenList extract_tokens(TokenList tl, TokenType open, TokenType close) {
  size_t start = find(tl, open); // Trova inizio blocco
  if (start >= tl.size) {
    fprintf(stderr, "Errore: blocco non trovato\n");
    return (TokenList){ .tokens = NULL, .size = 0 }; // Nessun blocco trovato
  }

  int brace_count = 1; // Conta i blocchi annidati
  size_t end = start + 1;

  while (end < tl.size && brace_count > 0) {
    if (tl.tokens[end].type == open) brace_count++;
    if (tl.tokens[end].type == close) brace_count--;

    end++;
  }

  if (brace_count != 0) {
    printf("Blocco non chiuso correttamente\n");
    return (TokenList){ .tokens = NULL, .size = 0 }; // Blocco non chiuso correttamente
  }

  // Slice tra '{' e '}', escludendo le parentesi
  return slice(tl, start + 1, (end - start - 2));
}


TokenList extract_block(TokenList tl) {
  TokenList tokens = extract_tokens(tl, TOKEN_LBRACE, TOKEN_RBRACE);
  if (tokens.tokens == NULL) {
    fprintf(stderr, "Errore: blocco { } non chiuso correttamente\n");
    exit(1);
  }
}

TokenList extract_parentheses(TokenList tl) {
  TokenList tokens = extract_tokens(tl, TOKEN_LPAREN, TOKEN_RPAREN);
  if (tokens.tokens == NULL) {
    fprintf(stderr, "Errore: parentesi ( ) non chiuse correttamente\n");
    exit(1);
  }
}

// TokenList extract_brackets(TokenList tl) {
//   TokenList tokens = extract_tokens(tl, TOKEN_LBRACKET, TOKEN_RBRACKET);
//   if (tokens.tokens == NULL) {
//     fprintf(stderr, "Errore: parentesi quadre [ ] non chiuse correttamente\n");
//     exit(1);
//   }
// }

int find_block_end(TokenList tl) {
  size_t start = find(tl, TOKEN_LBRACE); // Trova '{'
  if (start >= tl.size) return -1; // Nessun blocco trovato

  int brace_count = 1;
  int end = start + 1;

  while (end < tl.size && brace_count > 0) {
    if (tl.tokens[end].type == TOKEN_LBRACE) brace_count++;
    if (tl.tokens[end].type == TOKEN_RBRACE) brace_count--;

    end++;
  }

  if (brace_count != 0) {
    fprintf(stderr, "Errore: blocco { } non chiuso correttamente\n");
    exit(1);
  }

  return end - 1;
}

AST *function_definition(TokenList tl) {
  char *name = tl.tokens[1].value.value.sval; // Nome funzione
  int param_count = 0;
  char **params = malloc(sizeof(char *) * 20); // Max 20 parametri

  // Legge i parametri
  int i = 3;  // Salta "fun", il nome e (
  while (tl.tokens[i].type != TOKEN_RPAREN) {
    if (tl.tokens[i].type == TOKEN_IDENTIFIER) {
      params[param_count++] = tl.tokens[i].value.value.sval;
    }
    i++;
  }

  i++; // Salta il token =>

  // Analizza il blocco codice della funzione
  TokenList body = extract_block(tl);

  AST_BLOCK *block = parse_program(body, false);

  VarType r_type = VOID;

  // if (block->statements[block->count-1]->tag == TAG_RETURN) {
  //   printf("Calculating type for return tag\n");
  //   r_type = typeOf(block->statements[block->count-1]->data.ast_return.value);
  // } else if (find(body, TOKEN_RETURN) != -1) {
  //   printf("WARNING: The return statement shuld be the last operation of the function\n");
  // }

  // Crea il nodo funzione
  return new_ast_funct(r_type, name, NULL, params, param_count, block);  // ! RIMUOVERE I TIPI DI DATI
}

bool ends_with(const char *str, const char *suffix) {
  size_t str_len = strlen(str);
  size_t suffix_len = strlen(suffix);

  if (suffix_len > str_len) {
    return false;
  }

  return strcmp(str + (str_len - suffix_len), suffix) == 0;
}

AST *generate_tree(TokenList tl, bool parse_functions) {
  Variable *symbol_table = NULL;
  if (tl.size == 0 || tl.tokens == NULL) { return NULL; }

  AST *left, *right;  // Dichiarazioni per eventuali operatori binari
  VarType t1, t2;  // Used for type deduction

  // Controlla se è una dichiarazione di funzione
  if (tl.tokens[0].type == TOKEN_FUN && parse_functions) {
    return function_definition(tl);
  }

  /*
  if (i == 10) {
    Code
  }
  */
  if (tl.tokens[0].type == TOKEN_IF) {
    TokenList condition = extract_parentheses(tl);  // Isola il blocco di codice tra parentesi
    TokenList body = extract_block(tl);  // Isola il blocco di codice tra parentesi graffe

    return new_ast_if(generate_tree(condition, false), parse_program(body, false));
  }

  /*
  while (i < 10) {
    Code
  }
  */
  if (tl.tokens[0].type == TOKEN_WHILE) {
    TokenList condition = extract_parentheses(tl);  // Isola il blocco di codice tra parentesi
    TokenList body = extract_block(tl);  // Isola il blocco di codice tra parentesi graffe

    return new_ast_while(generate_tree(condition, false), parse_program(body, false));
  }

  /*
  for (@i = 0 to 10 step 1) {
    Code
  }
  */
  if (tl.tokens[0].type == TOKEN_FOR) {
    TokenList header = extract_parentheses(tl);
    TokenList body = extract_block(tl);

    printf("Header: \n");
    print_token_list(header);

    TokenList init = slice_from_to(header, 0, find(header, TOKEN_TO));
    TokenList to = slice_from_to(header, find(header, TOKEN_TO) + 1, find(header, TOKEN_STEP));
    TokenList step = slice_from_to(header, find(header, TOKEN_STEP) + 1, header.size);

    printf("Init: \n");
    print_token_list(init);
    printf("To: \n");
    print_token_list(to);
    printf("Step: \n");
    print_token_list(step);
    printf("Body: \n");
    print_token_list(body);

    AST *init_ast = generate_tree(init, false);     // Declare or assign
    AST *to_value_ast = generate_tree(to, false);   // End value (int or float)
    AST *step_ast = generate_tree(step, false);     // Step value (int or float)
    TokenType condition_type = -1;

    AST *start_value_ast = NULL;

    if (to_value_ast->tag != TAG_INT && to_value_ast->tag != TAG_FLOAT) {
      printf("Errore: valore di end non valido\n");
      exit(1);
    } else if (init_ast->tag != TAG_DECLARE && init_ast->tag != TAG_ASSIGN) {
      printf("Errore: inizializzazione del ciclo for non valida\n");
      exit(1);
    } else if (step_ast->tag != TAG_INT && step_ast->tag != TAG_FLOAT) {
      printf("Errore: valore di step non valido\n");
      exit(1);
    }

    if (init_ast->tag == TAG_DECLARE) {
      VarType type = init_ast->data.ast_declare.type;
      char *name = init_ast->data.ast_declare.name;
      start_value_ast = new_ast_var(type, name);
    } else if (init_ast->tag == TAG_ASSIGN) {
      char *name = init_ast->data.ast_assign.name;
      start_value_ast = new_ast_var(typeOf(init_ast->data.ast_assign.value), name);
    } else {
      printf("Errore: inizializzazione del ciclo for non valida\n");
      exit(1);
    }

    float step_value = 1;
    if (step_ast->tag == TAG_INT) {
      step_value = step_ast->data.ast_int.number;
    } else if (step_ast->tag == TAG_FLOAT) {
      step_value = step_ast->data.ast_float.number;
    }

    if (step_value < 0) {
      condition_type = TOKEN_GREATER_EQUALS;
    } else {
      condition_type = TOKEN_LESS;
    }

    AST *condition_ast = new_ast_condition(start_value_ast, to_value_ast, condition_type);

    return new_ast_for(init_ast, condition_ast, step_ast, parse_program(body, false));
  }

  for (int i = 0; i < tl.size; i++) {  
    Token *token = &tl.tokens[i];

    if (token->type == TOKEN_ASSIGN) {  // Operatore =
      TokenList sublist = { .tokens = &tl.tokens[i + 1], .size = tl.size - (i + 1) };
      if (tl.tokens[i-2].type == TOKEN_DECLARE) {
        AST *value = generate_tree(sublist, false);
        return new_ast_declare(typeOf(value), tl.tokens[i-1].value.value.sval, value);
      } else {
        return new_ast_assign(tl.tokens[i-1].value.value.sval, generate_tree(sublist, false));
      }
    }

    // Gestisce tutte le condizioni binarie
    if (token->type == TOKEN_EQUALS || 
        token->type == TOKEN_NOT_EQUALS || 
        token->type == TOKEN_LESS_EQUALS || 
        token->type == TOKEN_GREATER_EQUALS || 
        token->type == TOKEN_LESS || 
        token->type == TOKEN_GREATER) {
      TokenList operand1 = slice(tl, 0, i);
      TokenList operand2 = slice(tl, i + 1, tl.size - (i + 1));
      printf("trovato operatore binario\n");

      return new_ast_condition(generate_tree(operand1, false), generate_tree(operand2, false), tl.tokens[i].type);
    }
  }

  VarType exp_return_type;
  for (int i = 0; i < tl.size; i++) {  // Operatori {+, -}
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_SUM:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);

        if (t1 == INT && t2 == INT) {
          exp_return_type = INT;
        } else if (t1 == STRING && t2 == STRING) {
          exp_return_type = STRING;
        } else if (t1 == FLOAT && t2 == INT || t1 == INT && t2 == FLOAT || t1 == FLOAT && t2 == FLOAT) {
          exp_return_type = FLOAT;
        } else {
          printf("Errore: tipi non compatibili per l'operatore + \n");
          exit(1);
        }

        return new_ast_add(exp_return_type, left, right);
      break;
      case TOKEN_SUB:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);

        if (t1 == INT && t2 == INT) {
          exp_return_type = INT;
        } else if (t1 == FLOAT && t2 == INT || t1 == INT && t2 == FLOAT || t1 == FLOAT && t2 == FLOAT) {
          exp_return_type = FLOAT;
        } else {
          printf("Errore: tipi non compatibili per l'operatore -\n");
          exit(1);
        }
        return new_ast_sub(exp_return_type, left, right);
      break;
    }
  }
  
  for (int i = 0; i < tl.size; i++) {  // Operatori {*, /, %}
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_MUL:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);

        if (t1 == INT && t2 == INT) {
          exp_return_type = INT;
        } else if (t1 == FLOAT && t2 == INT || t1 == INT && t2 == FLOAT || t1 == FLOAT && t2 == FLOAT) {
          exp_return_type = FLOAT;
        } else if (t1 == STRING && t2 == INT || t1 == INT && t2 == STRING) {
          exp_return_type = STRING;
        } else {
          printf("Errore: tipi non compatibili per l'operatore *\n");
          exit(1);
        }
        return new_ast_mul(exp_return_type, left, right);
      break;
      case TOKEN_DIV:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);

        if (t1 == INT && t2 == INT) {
          exp_return_type = INT;
        } else if (t1 == FLOAT && t2 == INT || t1 == INT && t2 == FLOAT || t1 == FLOAT && t2 == FLOAT) {
          exp_return_type = FLOAT;
        } else {
          printf("Errore: tipi non compatibili per l'operatore /\n");
          exit(1);
        }
        return new_ast_div(exp_return_type, left, right);
      case TOKEN_PERCENT:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);

        if (t1 == INT && t2 == INT) {
          exp_return_type = INT;
        } else {
          printf("Errore: tipi non compatibili per l'operatore %%\n");
          exit(1);
        }
        return new_ast_mod(exp_return_type, left, right);
      break;
    }
  }

  // Gestisce le chiamate a funzione
  for (int i = 0; i < tl.size; i++) {
    if (tl.tokens[i].type == TOKEN_IDENTIFIER && tl.tokens[i + 1].type == TOKEN_LPAREN) {
      char *name = tl.tokens[i].value.value.sval;
      int arg_count = 0;
      AST **args = malloc(sizeof(AST *) * 20);

      TokenList args_tl = extract_parentheses(slice(tl, i + 1, tl.size - (i + 1)));
      
      // Cerca le virgole tenendo in considerazione la profondità della chiamata in modo da ignorare quelle delle chiamate annidate

      int paren_depth = 0;
      for (int j = 0; j < args_tl.size; j++) {
        if (args_tl.tokens[j].type == TOKEN_LPAREN) paren_depth++;
        if (args_tl.tokens[j].type == TOKEN_RPAREN) paren_depth--;

        if (paren_depth == 0 && args_tl.tokens[j].type == TOKEN_COMMA) {
          args[arg_count++] = generate_tree(slice(args_tl, 0, j), false);
          args_tl = slice(args_tl, j + 1, args_tl.size - (j + 1));
          j = 0;
        }
      }

      // Aggiunge l'ultimo argomento
      if (args_tl.size > 0) {
        args[arg_count++] = generate_tree(args_tl, false);
      }
      return new_ast_call(name, args, arg_count);
    }
  }

  if (tl.tokens[0].type == TOKEN_RETURN) {
    // Considera i token successivi come espressione di ritorno
    TokenList return_expr = { .tokens = tl.tokens + 1, .size = tl.size - 1 };
    AST *value = generate_tree(return_expr, false);  // Genera l'espressione che viene restituita
    return new_ast_return(value);  // Crea il nodo return
  }

  if (tl.tokens[0].type == TOKEN_FLOAT) {
    return new_ast_float(tl.tokens[0].value.value.fval);
  }

  if (tl.tokens[0].type == TOKEN_INTEGER) {
    return new_ast_int64(tl.tokens[0].value.value.i32val);
  }

  if (tl.tokens[0].type == TOKEN_IDENTIFIER) {
    return new_ast_var(INT, tl.tokens[0].value.value.sval);  // ! Usare una tabella dei tipi per determinare il tipo della variabile
  }

  if (tl.tokens[0].type == TOKEN_STRING) {
    return new_ast_string(tl.tokens[0].value.value.sval);
  }

  return NULL;
}

AST_BLOCK *concat_ast_blocks(AST_BLOCK *block1, AST_BLOCK *block2) {
  if (!block1) return block2;
  if (!block2) return block1;

  int new_count = block1->count + block2->count;
  AST **new_statements = malloc(sizeof(AST *) * new_count);

  // Copia le istruzioni del primo blocco
  for (int i = 0; i < block1->count; i++) {
    new_statements[i] = block1->statements[i];
  }

  // Copia le istruzioni del secondo blocco
  for (int i = 0; i < block2->count; i++) {
    new_statements[block1->count + i] = block2->statements[i];
  }

  // Alloca il nuovo blocco
  AST_BLOCK *new_block = malloc(sizeof(AST_BLOCK));
  new_block->statements = new_statements;
  new_block->count = new_count;

  // Libera le strutture originali (ma non i nodi AST)
  free(block1->statements);
  free(block2->statements);
  free(block1);
  free(block2);

  return new_block;
}

AST_BLOCK *define_all_functions(TokenList tl) {
  int fun_index = -1, block_end = -1;
  AST_BLOCK *functions = malloc(sizeof(AST_BLOCK));
  if (!functions) {
    fprintf(stderr, "Errore nell'allocazione della memoria per functions.\n");
    exit(1);
  }
  functions->statements = NULL;  // Inizializzazione della lista vuota
  functions->count = 0;

  do {
    fun_index = find(tl, TOKEN_FUN);    // Inizio della definizione di funzione
    if (fun_index == -1) break;

    block_end = find_block_end(slice(tl, fun_index, tl.size - fun_index));  // Cerca la } di chiusura

    if (block_end == -1) {
      fprintf(stderr, "Errore: parentesi graffa di chiusura non trovata.\n");
      exit(1);
    }

    TokenList sublist = slice(tl, fun_index, block_end - fun_index + 1);
    AST *function = generate_tree(sublist, true);

    // Aggiunge la funzione al blocco funzioni
    add_ast_to_block(functions, function);
    tl.tokens = &tl.tokens[block_end + 1];
    tl.size -= (block_end + 1);
  } while (fun_index != -1);

  return functions;
}

SYMBOL_TABLE *find_symbols(TokenList tl) {
  SYMBOL_TABLE *symbol_table = malloc(sizeof(SYMBOL_TABLE));
  int symbols = 0;

  for (int i = 0; i < tl.size; i++) {
    if (tl.tokens[i].type == TOKEN_DECLARE) {
      if (symbols == 0) {
        symbol_table->vars = malloc(sizeof(Variable));
      } else {
        symbol_table->vars = realloc(symbol_table->vars, sizeof(Variable) * (symbols+1));
      }

      symbol_table->vars[symbols].name = tl.tokens[i+1].value.value.sval;
      symbol_table->vars[symbols].type = INT;
      symbol_table->vars[symbols].value = NONE_VAL;
      
      symbols++;
    }
  }

  symbol_table->count = symbols;
  return symbol_table;
}


/*
parse_functions = false
  ritorna AST_BLOCK *

parse_functions = true
  ritorna PROGRAM *
  (AST_BLOCK + SYMBOL_TABLE)
*/
void *parse_program(TokenList tl, bool parse_functions) {
  if (tl.size == 0) return NULL;

  // Inizializza il blocco principale del programma
  AST_BLOCK *block = malloc(sizeof(AST_BLOCK));
  if (!block) {
    fprintf(stderr, "Errore nell'allocazione della memoria per AST_BLOCK\n");
    exit(1);
  }
  block->statements = NULL;
  block->count = 0;

  // Preprocessing
  if (tl.tokens[0].type == TOKEN_IMPORT) {
    char *filename = tl.tokens[1].value.value.sval;

    if (!ends_with(filename, ".h") && !ends_with(filename, ".c")) {
      fprintf(stderr, "Errore: il file da importare deve avere estensione .c o .h\n");
      exit(1);
    }
    AST *import = new_ast_system_import(filename);
    add_ast_to_block(block, import);
  }

  // Aggiunge tutte le funzioni definite all'inizio del programma
  if (parse_functions) {
    AST_BLOCK *functions = define_all_functions(tl);
    block = concat_ast_blocks(block, functions);

    SYMBOL_TABLE *symbol_table = find_symbols(tl);
    PROGRAM *program = malloc(sizeof(PROGRAM));
    *program = (PROGRAM) {block, symbol_table, symbol_table->count};
    return program;
  } else {
    // Analizza un blocco di codice
    int start = 0;

    for (int i = 0; i < tl.size; i++) {  // Itera per i token
      if (tl.tokens[i].type == TOKEN_IF || tl.tokens[i].type == TOKEN_WHILE || tl.tokens[i].type == TOKEN_FOR) {
        int end = find_block_end(slice(tl, i, tl.size - i));
        if (end == -1) {
          fprintf(stderr, "Errore: blocco if mai aperto\n");
          exit(1);
        }
        end += i;
        TokenList sublist = slice(tl, i, end - i + 1);
        add_ast_to_block(block, generate_tree(sublist, false));
        i = end;
        start = end + 1;
      }

      if (tl.tokens[i].type == TOKEN_EOL) { // Nuova riga
        TokenList sublist = { .tokens = &tl.tokens[start], .size = i - start };
        // Genera un albero AST per la sottolista di token
        add_ast_to_block(block, generate_tree(sublist, false));
        start = i + 1;
      }
    }
  }

  return block;
}

void print_tree(AST *node, int indent) {
  if (node == NULL) {
    return;
  }

  // Stampa l'indentazione
  for (int i = 0; i < indent; i++) {
    printf("|  ");
  }

  // Stampa il nodo in base al tipo
  switch (node->tag) {
    case TAG_ASSIGN:
      printf("Assign: %s\n", node->data.ast_assign.name);
      print_tree(node->data.ast_assign.value, indent + 1);
      break;
    case TAG_INT:
      printf("Int: %d\n", node->data.ast_int.number);
      break;
    case TAG_FLOAT:
      printf("Float: %f\n", node->data.ast_float.number);
      break;
    case TAG_STRING:
      printf("String: \"%s\"\n", node->data.ast_string.string);
      break;
    case TAG_VAR:
      printf("Variable: %s\n", node->data.ast_var.name);
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
    case TAG_MOD:
      printf("Module\n");
      print_tree(node->data.ast_div.left, indent + 1);
      print_tree(node->data.ast_div.right, indent + 1);
      break;
    case TAG_FUN:
      printf("\nFunction: %s (params: %d)\n", node->data.ast_funct.name, node->data.ast_funct.param_count);
      for (int i = 0; i < node->data.ast_funct.param_count; i++) {
        for (int j = 0; j < indent + 1; j++) printf("|  ");
        printf("Param: %s\n", node->data.ast_funct.params[i]);
      }
      // Stampa il blocco di codice
      if (node->data.ast_funct.body->count == 0 || node->data.ast_funct.body->statements == NULL) {
        printf("|  (Empty function body)\n");
      } else {
        for (int i = 0; i < node->data.ast_funct.body->count; i++) {
          print_tree(node->data.ast_funct.body->statements[i], indent + 1);
        }
      }
      
      break;
    case TAG_CALL:
      printf("Function Call: %s (args: %d)\n", node->data.ast_call.name, node->data.ast_call.arg_count);
      for (int i = 0; i < node->data.ast_call.arg_count; i++) {
        print_tree(node->data.ast_call.args[i], indent + 1);
      }
      break;
    case TAG_RETURN:
      printf("Return\n");
      print_tree(node->data.ast_return.value, indent + 1);
      break;
    case TAG_DECLARE:
      printf("Declare: %s\n", node->data.ast_declare.name);
      print_tree(node->data.ast_declare.value, indent + 1);
      break;
    case TAG_SYSTEM_IMPORT:
      printf("Import: %s\n", node->data.ast_system_import.filename);
      break;
    case TAG_IF:
      printf("If\n");
      print_tree(node->data.ast_if.condition, indent + 1);

      for (int i = 0; i < node->data.ast_if.body->count; i++) {
        print_tree(node->data.ast_if.body->statements[i], indent + 1);
      }
      break;
    case TAG_WHILE:
      printf("While\n");
      print_tree(node->data.ast_while.condition, indent + 1);

      for (int i = 0; i < node->data.ast_while.body->count; i++) {
        print_tree(node->data.ast_while.body->statements[i], indent + 1);
      }
      break;
    case TAG_FOR: 
      printf("For\n");
      print_tree(node->data.ast_for.init, indent + 1);
      print_tree(node->data.ast_for.condition, indent + 1);
      print_tree(node->data.ast_for.step, indent + 1);

      for (int i = 0; i < node->data.ast_for.body->count; i++) {
        print_tree(node->data.ast_for.body->statements[i], indent + 1);
      }
      break;
    case TAG_CONDITION:
      printf("Condition: ");
      switch (node->data.ast_condition.type) {
        case TOKEN_EQUALS:
          printf("'=='\n");
          break;
        case TOKEN_NOT_EQUALS:
          printf("'!='\n");
          break;
        case TOKEN_LESS:
          printf("'<'\n");
          break;
        case TOKEN_GREATER:
          printf("'>'\n");
          break;
        case TOKEN_LESS_EQUALS:
          printf("'<='\n");
          break;
        case TOKEN_GREATER_EQUALS:
          printf("'>='\n");
          break;
      }
      print_tree(node->data.ast_condition.left, indent + 1);
      print_tree(node->data.ast_condition.right, indent + 1);
      break;
    default:
      printf("Unknown node {%d}\n", node->tag);
      break;
  }
}
