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
    case TAG_CALL:
      t = ast->data.ast_call.ret_type;
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

TokenList extract_block(TokenList tl) {
  size_t start = find(tl, TOKEN_LBRACE); // Trova '{'
  if (start >= tl.size) return (TokenList){ .tokens = NULL, .size = 0 }; // Nessun blocco trovato

  int brace_count = 1; // Conta i blocchi annidati
  size_t end = start + 1;

  while (end < tl.size && brace_count > 0) {
    if (tl.tokens[end].type == TOKEN_LBRACE) brace_count++;
    if (tl.tokens[end].type == TOKEN_RBRACE) brace_count--;

    end++;
  }

  if (brace_count != 0) {
    fprintf(stderr, "Errore: blocco { } non chiuso correttamente\n");
    exit(1);
  }

  // Slice tra '{' e '}', escludendo le parentesi
  return slice(tl, start + 1, (end - start - 2));
}

AST *function_definition(TokenList tl) {
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

  // Analizza il blocco codice della funzione
  TokenList body = extract_block(tl);
  AST_BLOCK *block = parse_program(body, false);

  VarType r_type = VOID;

  if (block->statements[block->count-1]->tag == TAG_RETURN) {
    printf("Calculating type for return tag\n");
    r_type = typeOf(block->statements[block->count-1]->data.ast_return.value);
  } else if (find(body, TOKEN_RETURN) != -1) {
    printf("WARNING: The return statement shuld be the last operation of the function\n");
  }

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

  if (tl.tokens[0].type == TOKEN_STRING) {
    return new_ast_string(tl.tokens[0].value.value.sval);
  }

  if (tl.tokens[0].type == TOKEN_RETURN) {
    // Considera i token successivi come espressione di ritorno
    TokenList return_expr = { .tokens = &tl.tokens[1], .size = tl.size - 1 };
    AST *value = generate_tree(return_expr, false);  // Genera l'espressione che viene restituita
    return new_ast_return(value);  // Crea il nodo return
  }

  // Controlla se è una dichiarazione di funzione
  if (tl.tokens[0].type == TOKEN_FUN && parse_functions) {
    return function_definition(tl);
  }

  // Controlla se è una chiamata di funzione
  for (int i = 0; i < tl.size; i++) {
    if (tl.tokens[i].type == TOKEN_IDENTIFIER && tl.tokens[i + 1].type == TOKEN_LPAREN) {
      char *name = tl.tokens[i].value.value.sval;
      int arg_count = 0;
      AST **args = malloc(sizeof(AST *) * 6);

      int call_end = -1;  // Contiene l'indice della parentesi di chiusura
      int paren_depth = 0;

      printf("Cerco le parentesi di %s\n", name);
      for (int j = i + 1; j < tl.size; j++) {
        if (tl.tokens[j].type == TOKEN_LPAREN) {paren_depth++; printf("(");}
        if (tl.tokens[j].type == TOKEN_RPAREN) {paren_depth--; printf(")");}

        if (paren_depth == 0) {
          call_end = j;
          break;
        }
      }

      if (call_end == -1) {
        fprintf(stderr, "Errore: parentesi di chiusura non trovata nella chiamata a funzione: %s\n", name);
        exit(1);
      }

      printf("Inizio chiamata a %s: %d\n", name, i + 2);
      printf("Fine chiamata a %s: %d\n", name, call_end);

      int first_param = i + 2;
      int j = first_param;
      while (j < call_end) {
        if (tl.tokens[j].type == TOKEN_IDENTIFIER && tl.tokens[j + 1].type == TOKEN_LPAREN) {  // Chiamata a funzione
          printf("\tParametro (chiamata alla funzione): %s\n", tl.tokens[j].value.value.sval);
          int end = -1;

          paren_depth = 0;
          for (int h = j + 1; h < tl.size; h++) {
            if (tl.tokens[h].type == TOKEN_LPAREN) {paren_depth++; printf("(");}
            if (tl.tokens[h].type == TOKEN_RPAREN) {paren_depth--; printf(")");}

            if (paren_depth == 0) {
              end = h;
              break;
            }
          }

          args[arg_count++] = generate_tree(slice(tl, j, end + 1), false);
          j += end + 1;
          
        } else {
          printf("\tParametro (other): ");
          print_token_list(slice(tl, j, 1));

          int end = find_next_comma(tl, j);
          if (end == -1) end = call_end;
          args[arg_count++] = generate_tree(slice(tl, j, end - j), false);
          j = end;
        }
        
        printf("argomenti: %d\n", arg_count);
        printf("call_end: %d\n", call_end);
        printf("j: %d\n", j);
        printf("tl.tokens[j].type: %d\n", tl.tokens[j].type);
      }

      return new_ast_call(name, args, arg_count);
    }

  }

  for (int i = 0; i < tl.size; i++) {  // Operatore =
    Token *token = &tl.tokens[i];

    if (token->type == TOKEN_ASSIGN) {
      TokenList sublist = { .tokens = &tl.tokens[i + 1], .size = tl.size - (i + 1) };
      if (tl.tokens[i-2].type == TOKEN_DECLARE) {
        AST *value = generate_tree(sublist, false);
        return new_ast_declare(typeOf(value), tl.tokens[i-1].value.value.sval, value);
      } else {
        return new_ast_assign(tl.tokens[i-1].value.value.sval, generate_tree(sublist, false));
      }
    }
  }

  for (int i = 0; i < tl.size; i++) {  // Operatori {+, -}
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_SUM:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        VarType t1 = typeOf(left), t2 = typeOf(right);  // Type deduction
        return new_ast_add((t1 == INT && t2 == INT) ? INT : FLOAT, left, right);
      break;
      case TOKEN_SUB:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);
        return new_ast_sub((t1 == INT && t2 == INT) ? INT : FLOAT, left, right);
      break;
    }
  }
  
  for (int i = 0; i < tl.size; i++) {  // Operatori {*, /}
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_MUL:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);
        return new_ast_mul((t1 == INT && t2 == INT) ? INT : FLOAT, left, right);
      break;
      case TOKEN_DIV:
        left = generate_tree((TokenList) {tl.tokens, i}, false); // Prende i token fino all'operatore
        right = generate_tree((TokenList) {tl.tokens + i + 1, tl.size - (i + 1)}, false); // Token dopo l'operatore
        t1 = typeOf(left);
        t2 = typeOf(right);
        return new_ast_div((t1 == INT && t2 == INT) ? INT : FLOAT, left, right);
      break;
    }
  }

  for (int i = 0; i < tl.size; i++) {  // Casi base (dichiarazioni, nomi variabili, numeri costanti)
    Token *token = &tl.tokens[i];

    switch (token->type) {
      case TOKEN_IDENTIFIER: {
        char *identifier = token->value.value.sval;
        // Per adesso ogni identificatore è considerato come nome di variabile perchè non ci sono ancora operatori
        return new_ast_var(INT, identifier);  // ! Usare una tabella dei tipi per determinare il tipo della variabile
      }
      break;
      case TOKEN_NUMBER:  // TODO: dividere TOKEN_NUMBER in TOKEN_INT e TOKEN_FLOAT
        return new_ast_int64(token->value.value.i32val);
      break;
    }
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

    // TODO: tenere in condiserazione la profondità delle parentesi
    block_end = find(tl, TOKEN_RBRACE); // Fine della definizione di funzione con }

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

parse_functions = true
  ritorna AST_BLOCK *

parse_functions = false
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
    // Analizza il resto del programma
    int start = 0;
    for (int i = 0; i < tl.size; i++) {  // Itera per i token
      if (tl.tokens[i].type == TOKEN_EOL) { // Nuova riga
        TokenList sublist = { .tokens = &tl.tokens[start], .size = i - start };
        // Genera un albero AST per la sottolista di token e aggiungilo al blocco
        add_ast_to_block(block, generate_tree(sublist, false));
        start = i + 1;
      }
    }

    // Se c'è ancora un'espressione non processata (l'ultima riga)
    if (start < tl.size) {
      TokenList sublist = { .tokens = &tl.tokens[start], .size = tl.size - start };
      add_ast_to_block(block, generate_tree(sublist, false));
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
    printf("  ");
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
    case TAG_FUN:
      printf("\nFunction: %s (params: %d)\n", node->data.ast_funct.name, node->data.ast_funct.param_count);
      for (int i = 0; i < node->data.ast_funct.param_count; i++) {
        for (int j = 0; j < indent + 1; j++) printf("  ");
        printf("Param: %s\n", node->data.ast_funct.params[i]);
      }
      // Stampa il blocco di codice
      if (node->data.ast_funct.body->count == 0 || node->data.ast_funct.body->statements == NULL) {
        printf("  (Empty function body)\n");
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
    default:
      printf("Unknown node {%d}\n", node->tag);
      break;
  }
}
