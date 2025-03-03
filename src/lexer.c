#include "lexer.h"

#define NUM_TOKENS (sizeof(TOKEN_PATTERNS) / sizeof(TOKEN_PATTERNS[0]))

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


TokenList tokenize_code(const char *code) {
  pcre2_code *regex[NUM_TOKENS];
  pcre2_match_data *match_data;
  PCRE2_SIZE error_offset;
  int error_code, i, pos = 0, len = strlen(code);
  Token *tokens;

  // Compila tutti i pattern
  for (i = 0; i < NUM_TOKENS; i++) {
    regex[i] = pcre2_compile((PCRE2_SPTR)TOKEN_PATTERNS[i], PCRE2_ZERO_TERMINATED, 0, &error_code, &error_offset, NULL);
    if (!regex[i]) {
      printf("Errore nella compilazione del regex %s\n", TOKEN_PATTERNS[i]);
      return (TokenList) {NULL, 0};
    }
  }

  match_data = pcre2_match_data_create_from_pattern(regex[0], NULL);

  int tokens_found = 0;

  while (pos < len) {
    int best_match = -1;
    int best_start = len, best_end = len;

    // Cerca il miglior match tra i pattern
    for (i = 0; i < NUM_TOKENS; i++) {
      int rc = pcre2_match(regex[i], (PCRE2_SPTR)(code + pos), len - pos, 0, 0, match_data, NULL);
      if (rc >= 0) {
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        int start = (int)ovector[0];
        int end = (int)ovector[1];

        if (start == 0 && (best_match == -1 || end < best_end)) {
          best_match = i;
          best_start = start;
          best_end = end;
        }
      }
    }

    if (best_match != -1) {
      tokens_found++;
      int length = best_end - best_start;

      char *value = malloc(length + 1);
      strncpy(value, code + pos, length);
      value[length] = '\0';

      if (tokens_found > 1) {
        tokens = realloc(tokens, sizeof(Token) * tokens_found);
      } else {
        tokens = malloc(sizeof(Token));
      }

      switch (best_match) {
        case TOKEN_FUN:
          tokens[tokens_found - 1] = (Token) {TOKEN_FUN, NONE_VAL, best_start, length};
          break;
        case TOKEN_ARROW:
          tokens[tokens_found - 1] = (Token) {TOKEN_ARROW, NONE_VAL, best_start, length};
          break;
        case TOKEN_RETURN:
          tokens[tokens_found - 1] = (Token) {TOKEN_RETURN, NONE_VAL, best_start, length};
          break;
        case TOKEN_SUM:
          tokens[tokens_found - 1] = (Token) {TOKEN_SUM, NONE_VAL, best_start, length};
          break;
        case TOKEN_SUB:
          tokens[tokens_found - 1] = (Token) {TOKEN_SUB, NONE_VAL, best_start, length};
          break;
        case TOKEN_MUL:
          tokens[tokens_found - 1] = (Token) {TOKEN_MUL, NONE_VAL, best_start, length};
          break;
        case TOKEN_DIV:
          tokens[tokens_found - 1] = (Token) {TOKEN_DIV, NONE_VAL, best_start, length};
          break;
        case TOKEN_ASSIGN:
          tokens[tokens_found - 1] = (Token) {TOKEN_ASSIGN, NONE_VAL, best_start, length};
          break;
        case TOKEN_LPAREN:
          tokens[tokens_found - 1] = (Token) {TOKEN_LPAREN, NONE_VAL, best_start, length};
          break;
        case TOKEN_RPAREN:
          tokens[tokens_found - 1] = (Token) {TOKEN_RPAREN, NONE_VAL, best_start, length};
          break;
        case TOKEN_LBRACE:
          tokens[tokens_found - 1] = (Token) {TOKEN_LBRACE, NONE_VAL, best_start, length};
          break;
        case TOKEN_RBRACE:
          tokens[tokens_found - 1] = (Token) {TOKEN_RBRACE, NONE_VAL, best_start, length};
          break;
        case TOKEN_DECLARE:
          tokens[tokens_found - 1] = (Token) {TOKEN_DECLARE, NONE_VAL, best_start, length};
          break;
        case TOKEN_NUMBER:
          tokens[tokens_found - 1] = (Token) {TOKEN_NUMBER, {atoi(value), 0}, best_start, length};
          break;
        case TOKEN_STRING:
          tokens[tokens_found - 1] = (Token) {TOKEN_STRING, {.value.sval = value, 0}, best_start, length};
          break;
        case TOKEN_IDENTIFIER:
          tokens[tokens_found - 1] = (Token) {TOKEN_IDENTIFIER, {.value.sval = value, 0}, best_start, length};
          break;
        case TOKEN_EOL:
          tokens[tokens_found - 1] = (Token) {TOKEN_EOL, NONE_VAL, best_start, length};
          break;
        default:
          printf("Unknown token at position %d, length %d\n", best_start, length);
          break;
      }

      pos += best_end;
    } else {
      pos++; // Salta caratteri non riconosciuti
    }
  }

  // Pulisce la memoria
  for (i = 0; i < NUM_TOKENS; i++) pcre2_code_free(regex[i]);
  pcre2_match_data_free(match_data);

  return (TokenList) {tokens, tokens_found};
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