#include "lexer.h"

const char* TOKEN_PATTERNS[] = {
  "\\bfun\\b",     // TOKEN_FUN
  "=>",            // TOKEN_ARROW
  "<-",            // TOKEN_RETURN
  "\\+",           // TOKEN_SUM
  "-",             // TOKEN_SUB
  "\\*",           // TOKEN_MUL
  "/",             // TOKEN_DIV
  "=",             // TOKEN_ASSIGN
  "\\(",           // TOKEN_LPAREN
  "\\)",           // TOKEN_RPAREN
  "{",             // TOKEN_LBRACE
  "}",             // TOKEN_RBRACE
  "@",             // TOKEN_DECLARE
  "-?\\b\\d+(\\.\\d+)?\\b", // TOKEN_NUMBER
  "\"((\\\\.|[^\"])*)\"", // TOKEN_STRING
  "\\b[a-zA-Z_][a-zA-Z0-9_]*\\b", // TOKEN_IDENTIFIER
  ";",            // TOKEN_EOL
};


const char *TOKEN_NAMES[] = {
  "TOKEN_FUN",
  "TOKEN_ARROW",
  "TOKEN_RETURN",
  "TOKEN_SUM",
  "TOKEN_SUB",
  "TOKEN_MUL",
  "TOKEN_DIV",
  "TOKEN_ASSIGN",
  "TOKEN_LPAREN",
  "TOKEN_RPAREN",
  "TOKEN_LBRACE",
  "TOKEN_RBRACE",
  "TOKEN_DECLARE",
  "TOKEN_NUMBER",
  "TOKEN_STRING",
  "TOKEN_IDENTIFIER",
  "TOKEN_EOL"
};

Token *new_token(TokenType type, Value value) {
  Token *token = malloc(sizeof(Token));
  if (token != NULL) {
    token->type = type;
    token->value = value;
  }
  return token;
}

// Token *find_first_token(char *code) {
//   const int num_patterns = sizeof(TOKEN_PATTERNS) / sizeof(TOKEN_PATTERNS[0]);
//   pcre2_code *regexes[num_patterns];
//   pcre2_match_data *match_data;
//   int min_pos = -1;
//   int min_index = -1;
//   Token *token = malloc(sizeof(Token));

//   // Compile all regular expressions
//   for (int i = 0; i < num_patterns; i++) {
//     int errorcode;
//     PCRE2_SIZE erroroffset;
//     regexes[i] = pcre2_compile(
//       (PCRE2_SPTR)TOKEN_PATTERNS[i],
//       PCRE2_ZERO_TERMINATED,
//       0,
//       &errorcode,
//       &erroroffset,
//       NULL
//     );
//     if (regexes[i] == NULL) {
//       PCRE2_UCHAR buffer[256];
//       pcre2_get_error_message(errorcode, buffer, sizeof(buffer));
//       fprintf(stderr, "PCRE2 compilation failed at pattern %d: %s\n", i, buffer);
//       exit(1);
//     }
// }

//   // Search for the first occurrence of each pattern
//   for (int i = 0; i < num_patterns; i++) {
//     match_data = pcre2_match_data_create_from_pattern(regexes[i], NULL);
//     int rc = pcre2_match(
//       regexes[i],
//       (PCRE2_SPTR)code,
//       strlen(code),
//       0,
//       0,
//       match_data,
//       NULL
//     );
//     if (rc >= 0) {
//       PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
//       int pos = (int)ovector[0];
//       if (min_pos == -1 || pos < min_pos) {
//         min_pos = pos;
//         min_index = i;
//       }
//     }
//     pcre2_match_data_free(match_data);
//   }

//   if (min_index != -1) {
//     match_data = pcre2_match_data_create_from_pattern(regexes[min_index], NULL);
//     pcre2_match(
//       regexes[min_index],
//       (PCRE2_SPTR)code,
//       strlen(code),
//       0,
//       0,
//       match_data,
//       NULL
//     );
//     PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
//     int start = (int)ovector[0];
//     int end = (int)ovector[1];
//     int length = end - start;

//     char *value = malloc(length + 1);
//     strncpy(value, code + start, length);
//     value[length] = '\0';

//     switch (min_index) {
//       case TOKEN_SUM:
//         printf("Matched TOKEN_SUM at position %d, length %d\n", start, length);
//         *token = (Token) {TOKEN_SUM, NONE_VAL, start, length};
//         break;
//       case TOKEN_SUB:
//         printf("Matched TOKEN_SUB at position %d, length %d\n", start, length);
//         *token = (Token) {TOKEN_SUB, NONE_VAL, start, length};
//         break;
//       case TOKEN_MUL:
//         printf("Matched TOKEN_MUL at position %d, length %d\n", start, length);
//         *token = (Token) {TOKEN_MUL, NONE_VAL, start, length};
//         break;
//       case TOKEN_DIV:
//         printf("Matched TOKEN_DIV at position %d, length %d\n", start, length);
//         *token = (Token) {TOKEN_DIV, NONE_VAL, start, length};
//         break;
//       case TOKEN_ASSIGN:
//         printf("Matched TOKEN_ASSIGN at position %d, length %d\n", start, length);
//         *token = (Token) {TOKEN_ASSIGN, NONE_VAL, start, length};
//         break;
//       case TOKEN_NUMBER:
//         printf("Matched TOKEN_NUMBER (%s) at position %d, length %d\n", value, start, length);
//         *token = (Token) {TOKEN_NUMBER, {atoi(value), 0}, start, length};
//         break;
//       case TOKEN_STRING:
//         printf("Matched TOKEN_STRING (%s) at position %d, length %d\n", value, start, length);
//         *token = (Token) {TOKEN_STRING, {.value.sval = value, 0}, start, length};
//         break;
//       case TOKEN_IDENTIFIER:
//         printf("Matched TOKEN_IDENTIFIER (%s) at position %d, length %d\n", value, start, length);
//         *token = (Token) {TOKEN_IDENTIFIER, {.value.sval = value, 0}, start, length};
//         break;
//       case TOKEN_EOL:
//         printf("Matched TOKEN_EOL at position %d, length %d\n", start, length);
//         *token = (Token) {TOKEN_EOL, NONE_VAL, start, length};
//         break;
//       default:
//         printf("Unknown token at position %d, length %d\n", start, length);
//         break;
//     }

//     pcre2_match_data_free(match_data);
//   } else {
//     printf("No pattern found in this code.\n");
//     //free(token);
//     return NULL;
//   }

//   // Free the memory allocated for the regular expressions
//   for (int i = 0; i < num_patterns; i++) {
//     pcre2_code_free(regexes[i]);
//   }

//   return token;
// }

Token *find_first_token(char *code) {
  const int num_patterns = sizeof(TOKEN_PATTERNS) / sizeof(TOKEN_PATTERNS[0]);
  pcre2_code *regexes[num_patterns];
  pcre2_match_data *match_data;
  int min_pos = -1;
  int min_index = -1;
  Token *token = malloc(sizeof(Token));
  int current_pos = 0; // Position tracker for next search
  
  // Compile all regular expressions
  for (int i = 0; i < num_patterns; i++) {
    int errorcode;
    PCRE2_SIZE erroroffset;
    regexes[i] = pcre2_compile(
      (PCRE2_SPTR)TOKEN_PATTERNS[i],
      PCRE2_ZERO_TERMINATED,
      0,
      &errorcode,
      &erroroffset,
      NULL
    );
    if (regexes[i] == NULL) {
      PCRE2_UCHAR buffer[256];
      pcre2_get_error_message(errorcode, buffer, sizeof(buffer));
      fprintf(stderr, "PCRE2 compilation failed at pattern %d: %s\n", i, buffer);
      exit(1);
    }
  }

  // Search for the first occurrence of each pattern, starting from current_pos
  while (code[current_pos] != '\0') {
    min_pos = -1; // Reset for each new token
    min_index = -1;

    for (int i = 0; i < num_patterns; i++) {
      match_data = pcre2_match_data_create_from_pattern(regexes[i], NULL);
      int rc = pcre2_match(
        regexes[i],
        (PCRE2_SPTR)(code + current_pos),
        strlen(code) - current_pos,
        0,
        0,
        match_data,
        NULL
      );
      if (rc >= 0) {
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        int pos = (int)ovector[0] + current_pos;
        if (min_pos == -1 || pos < min_pos) {
          min_pos = pos;
          min_index = i;
        }
      }
      pcre2_match_data_free(match_data);
    }

    if (min_index != -1) {
      match_data = pcre2_match_data_create_from_pattern(regexes[min_index], NULL);
      pcre2_match(
        regexes[min_index],
        (PCRE2_SPTR)(code + current_pos),
        strlen(code) - current_pos,
        0,
        0,
        match_data,
        NULL
      );
      PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
      int start = (int)ovector[0] + current_pos;
      int end = (int)ovector[1] + current_pos;
      int length = end - start;

      char *value = malloc(length + 1);
      strncpy(value, code + start, length);
      value[length] = '\0';

      // Create the token and move the position forward
      switch (min_index) {
        case 0: // TOKEN_FUN
          *token = (Token) {TOKEN_FUN, NONE_VAL, start, length};
          break;
        case 1: // TOKEN_ARROW
          *token = (Token) {TOKEN_ARROW, NONE_VAL, start, length};
          break;
        case 2: // TOKEN_RETURN
          *token = (Token) {TOKEN_RETURN, NONE_VAL, start, length};
          break;
        case 3: // TOKEN_SUM
          *token = (Token) {TOKEN_SUM, NONE_VAL, start, length};
          break;
        case 4: // TOKEN_SUB
          *token = (Token) {TOKEN_SUB, NONE_VAL, start, length};
          break;
        case 5: // TOKEN_MUL
          *token = (Token) {TOKEN_MUL, NONE_VAL, start, length};
          break;
        case 6: // TOKEN_DIV
          *token = (Token) {TOKEN_DIV, NONE_VAL, start, length};
          break;
        case 7: // TOKEN_ASSIGN
          *token = (Token) {TOKEN_ASSIGN, NONE_VAL, start, length};
          break;
        case 8: // TOKEN_LPAREN
          *token = (Token) {TOKEN_LPAREN, NONE_VAL, start, length};
          break;
        case 9: // TOKEN_RPAREN
          *token = (Token) {TOKEN_RPAREN, NONE_VAL, start, length};
          break;
        case 10: // TOKEN_LBRACE
          *token = (Token) {TOKEN_LBRACE, NONE_VAL, start, length};
          break;
        case 11: // TOKEN_RBRACE
          *token = (Token) {TOKEN_RBRACE, NONE_VAL, start, length};
          break;
        case 12: // TOKEN_DECLARE
          *token = (Token) {TOKEN_DECLARE, NONE_VAL, start, length};
          break;
        case 13: // TOKEN_NUMBER
          *token = (Token) {TOKEN_NUMBER, {atoi(value), 0}, start, length};
          break;
        case 14: // TOKEN_STRING
          *token = (Token) {TOKEN_STRING, {.value.sval = value, 0}, start, length};
          break;
        case 15: // TOKEN_IDENTIFIER
          *token = (Token) {TOKEN_IDENTIFIER, {.value.sval = value, 0}, start, length};
          break;
        case 16: // TOKEN_EOL
          *token = (Token) {TOKEN_EOL, NONE_VAL, start, length};
          break;
        default:
          printf("Unknown token at position %d, length %d\n", start, length);
          break;
      }

      current_pos = end;  // Move past the matched token
      pcre2_match_data_free(match_data);
    } else {
      printf("No pattern found in this code.\n");
      free(token);
      return NULL;
    }
  }

  // Free the memory allocated for the regular expressions
  for (int i = 0; i < num_patterns; i++) {
    pcre2_code_free(regexes[i]);
  }

  return token;
}

void print_token_list(TokenList tl) {
  if (tl.tokens == NULL || tl.size == 0) {
    printf("Token list is empty or uninitialized.\n");
    return;
  }

  for (int i = 0; i < tl.size; i++) {
    if (tl.tokens[i].type < 0 || tl.tokens[i].type >= sizeof(TOKEN_NAMES) / sizeof(TOKEN_NAMES[0])) {
      printf("Invalid token at index %d\n", i);
      continue;
    }

    printf("(Token) {%s, value: ", TOKEN_NAMES[tl.tokens[i].type]);
    if (tl.tokens[i].type == TOKEN_STRING || tl.tokens[i].type == TOKEN_IDENTIFIER) {
      printf("%s}\n", tl.tokens[i].value.value.sval);
    } else {
      if (!tl.tokens[i].value.is_none) {
        printf("%d}\n", tl.tokens[i].value.value.i32val);
      } else {
        printf("NONE}\n");
      }
    }
  }
}


int _strlen(char *str) {
  int i;
  for (i = 0; str[i] != '\0'; i++);
  return i;
}

// TokenList analyze_code(char *code) {
//   // Allocate first token
//   Token *tokens = malloc(sizeof(Token));
//   int elements = 1;

//   int i = 0;
//   int size = _strlen(code);

//   if (tokens != NULL) {
//     while (i < size) {
//       // Last position
//       Token *next_token = find_first_token(code);
//       if (next_token == NULL) {
//         break; // Evita di procedere se non ci sono più token
//       }
//       tokens[elements - 1] = *next_token;

//       code += tokens[elements - 1].start + tokens[elements - 1].length;
//       i += tokens[elements - 1].start + tokens[elements - 1].length;

//       if (!(tokens = realloc(tokens, (elements+1) * sizeof(Token)))) {
//         fprintf(stderr, "Error in memory allocation");
//         exit(1);
//       }
//       elements++;
//     }
    
//     //free(tokens + elements);  // Deallocate last space
//     elements--;
//   }

//   return (TokenList){tokens, elements};
// }


TokenList analyze_code(char *code) {
  int size = _strlen(code);
  int offset = 0;
  TokenList token_list;
  token_list.tokens = NULL;
  token_list.size = 0;

  while (offset < size) {
    Token *next_token = find_first_token(code + offset);
    if (next_token == NULL) {
      break; // Nessun altro token trovato, esci dal ciclo
    }

    // Aggiungi il nuovo token alla lista
    token_list.size++;
    token_list.tokens = realloc(token_list.tokens, token_list.size * sizeof(Token));
    if (!token_list.tokens) {
      fprintf(stderr, "Errore di allocazione della memoria\n");
      exit(1);
    }
    token_list.tokens[token_list.size - 1] = *next_token;

    // Avanza nell'input in base alla posizione e lunghezza del token trovato
    offset += next_token->start + next_token->length;

    free(next_token); // Libera la memoria del token temporaneo
  }

  return token_list;
}