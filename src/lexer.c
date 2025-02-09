#include "lexer.h"

const char* TOKEN_PATTERNS[] = {
  // Operatori
  "\\+",          // TOKEN_SUM: +
  "-",            // TOKEN_SUB: -
  "\\*",          // TOKEN_MUL: *
  "/",            // TOKEN_DIV: /
  "=",            // TOKEN_ASSIGN: =

  // Valori costanti
  "\\b[0-9]+\\b",               // TOKEN_NUMBER: sequenza di cifre
  "\"((\\\\.|[^\"])*)\"",       // TOKEN_STRING: stringa tra virgolette

  "\\b[a-zA-Z_][a-zA-Z0-9_]*\\b(?=(?:[^\"]*\"[^\"]*\")*[^\"]*$)",  // TOKEN_IDENTIFIER: nomi variabili

  // Fine linea
  ";",            // TOKEN_EOL: punto e virgola
};

const char *TOKEN_NAMES[] = {
  "TOKEN_SUM",
  "TOKEN_SUB",
  "TOKEN_MUL",
  "TOKEN_DIV",
  "TOKEN_ASSIGN",
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

Token *find_first_token(char *code) {
  const int num_patterns = sizeof(TOKEN_PATTERNS) / sizeof(TOKEN_PATTERNS[0]);
  pcre2_code *regexes[num_patterns];
  pcre2_match_data *match_data;
  int min_pos = -1;
  int min_index = -1;
  Token *token = malloc(sizeof(Token));

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

  // Search for the first occurrence of each pattern
  for (int i = 0; i < num_patterns; i++) {
    match_data = pcre2_match_data_create_from_pattern(regexes[i], NULL);
    int rc = pcre2_match(
      regexes[i],
      (PCRE2_SPTR)code,
      strlen(code),
      0,
      0,
      match_data,
      NULL
    );
    if (rc >= 0) {
      PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
      int pos = (int)ovector[0];
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
      (PCRE2_SPTR)code,
      strlen(code),
      0,
      0,
      match_data,
      NULL
    );
    PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
    int start = (int)ovector[0];
    int end = (int)ovector[1];
    int length = end - start;

    char *value = malloc(length + 1);
    strncpy(value, code + start, length);
    value[length] = '\0';

    switch (min_index) {
      case TOKEN_SUM:
        *token = (Token) {TOKEN_SUM, NONE_VAL, start, length};
        break;
      case TOKEN_SUB:
        *token = (Token) {TOKEN_SUB, NONE_VAL, start, length};
        break;
      case TOKEN_MUL:
        *token = (Token) {TOKEN_MUL, NONE_VAL, start, length};
        break;
      case TOKEN_DIV:
        *token = (Token) {TOKEN_DIV, NONE_VAL, start, length};
        break;
      case TOKEN_ASSIGN:
        *token = (Token) {TOKEN_ASSIGN, NONE_VAL, start, length};
        break;
      case TOKEN_NUMBER:
        *token = (Token) {TOKEN_NUMBER, {atoi(value), 0}, start, length};
        break;
      case TOKEN_STRING:
        *token = (Token) {TOKEN_STRING, {.value.sval = value, 0}, start, length};
        break;
      case TOKEN_IDENTIFIER:
        *token = (Token) {TOKEN_IDENTIFIER, {.value.sval = value, 0}, start, length};
        break;
      case TOKEN_EOL:
        *token = (Token) {TOKEN_EOL, NONE_VAL, start, length};
        break;
      default:
        break;
    }
    pcre2_match_data_free(match_data);
  } else {
    printf("No pattern found in this code.\n");
  }

  // Free the memory allocated for the regular expressions
  for (int i = 0; i < num_patterns; i++) {
    pcre2_code_free(regexes[i]);
  }

  return token;
}

void print_token_list(TokenList tl) {
  Token *tokens = tl.tokens;

  for (int i = 0; i < tl.size; i++) {
    printf("(Token) {%s, value: ", TOKEN_NAMES[tokens[i].type]);

    if (tokens[i].type == TOKEN_STRING || tokens[i].type == TOKEN_IDENTIFIER) {
      printf("%s}\n", tokens[i].value.value.sval);
    } else {
      if (!tokens[i].value.is_none) {
        printf("%d}\n", tokens[i].value.value);
      } else {
        printf("NONE}\n");
      }
    }
  }
}

int _sizeof(char *str) {
  int i;
  for (i = 0; str[i] != '\0'; i++);
  return i;
}

TokenList analyze_code(char *code) {
  // Allocate first token
  Token *tokens = malloc(sizeof(Token));
  int elements = 1;

  int i = 0;
  int size = _sizeof(code);

  if (tokens != NULL) {
    while (i < size) {
      // Last position
      tokens[elements - 1] = *find_first_token(code);

      code += tokens[elements - 1].start + tokens[elements - 1].length;
      i += tokens[elements - 1].start + tokens[elements - 1].length;

      if (!(tokens = realloc(tokens, (elements+1) * sizeof(Token)))) {
        fprintf(stderr, "Error in memory allocation");
        exit(1);
      }
      elements++;
    }
    
    //free(tokens + elements);  // Deallocate last space
    elements--;
  }

  return (TokenList){tokens, elements};
}