#pragma once
#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <string.h>  // strncpy()
#include <stdio.h>

// To remember: POSIX regex.h sucks
// #include <regex.h>

#define PCRE2_CODE_UNIT_WIDTH 8  // For pcre2
#include <pcre2.h>

typedef struct {
  union {
    char cval;    // char
    long i64val;  // int 64
    int i32val;   // int 32
    short i16val; // int 16
    float fval;   // float
    char *sval;   // string
  } value;
  int is_none;
} Value;


/*
for (i = 0 to 10 step 1) {
  # Blocco di codice
}
*/

typedef enum {
  // Keywords
  TOKEN_IMPORT,  // import
  TOKEN_FUN,     // fun
  TOKEN_RETURN,  // <-
  TOKEN_IF,      // if
  // TODO: implementare blocco else
  TOKEN_ELSE,    // else
  TOKEN_WHILE,   // while
  // TODO: implementare istruzione break e continue
  TOKEN_FOR,     // for
  TOKEN_TO,      // to
  TOKEN_STEP,    // step

  TOKEN_EQUALS,           // ==
  TOKEN_NOT_EQUALS,       // !=
  TOKEN_LESS_EQUALS,      // <=
  TOKEN_GREATER_EQUALS,   // >=
  TOKEN_LESS,             // <
  TOKEN_GREATER,          // >

  TOKEN_ARROW,   // =>

  // Operators
  TOKEN_SUM,     // +
  TOKEN_SUB,     // -
  TOKEN_MUL,     // *
  TOKEN_DIV,     // /
  TOKEN_ASSIGN,  // =

  // Delimiters
  TOKEN_LPAREN,  // (
  TOKEN_RPAREN,  // )
  TOKEN_LBRACE,  // {
  TOKEN_RBRACE,  // }
  TOKEN_DECLARE, // @

  // Constants and Identifiers
  TOKEN_NUMBER,      // 123
  TOKEN_STRING,      // "hello"
  TOKEN_IDENTIFIER,  // variable name, function name

  TOKEN_EOL,         // ;
  TOKEN_COMMA,       // ,
} TokenType;

#define NONE_VAL (Value) {0, 1}

typedef struct {
  TokenType type;
  Value value;
  int start;
  int length;
} Token;

typedef struct {
  Token *tokens;
  int size;
} TokenList;

TokenList tokenize_code(const char *input);
void print_token_list(TokenList tl);

#endif