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

typedef enum {
  // Operators
  TOKEN_SUM,     // +
  TOKEN_SUB,     // -
  TOKEN_MUL,     // *
  TOKEN_DIV,     // /
  TOKEN_ASSIGN,  // =

  // Constant values
  TOKEN_NUMBER,  // 123
  TOKEN_STRING,  // "hello"

  TOKEN_IDENTIFIER,    // variable name, type name
  TOKEN_EOL            // ;
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

TokenList analyze_code(char *code);
void print_token_list(TokenList tl);

#endif