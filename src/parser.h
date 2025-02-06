#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include "lexer.h"  // Value

typedef struct AST AST;

typedef struct { char *name; AST *value; } AST_ASSIGN;
typedef struct { int number; } AST_NUMBER;

typedef struct { AST *left; AST *right; } AST_ADD;
typedef struct { AST *left; AST *right; } AST_SUB;
typedef struct { AST *left; AST *right; } AST_MUL;
typedef struct { AST *left; AST *right; } AST_DIV;

typedef enum {
  TAG_ASSIGN,
  TAG_NUMBER,
  TAG_ADD,
  TAG_SUB,
  TAG_MUL,
  TAG_DIV,
} AST_TAG;

typedef struct AST {
  AST_TAG tag;

  union {
    AST_ASSIGN ast_assign;
    AST_NUMBER ast_number;
    
    AST_ADD ast_add;
    AST_SUB ast_sub;
    AST_MUL ast_mul;
    AST_DIV ast_div;
  } data;
} AST;

typedef enum {
  CHAR,
  INT16,
  INT32,
  INT64,
  FLOAT,
  STRING,
  NONE
} VarType;

typedef struct {
  char *name;
  VarType type;
  Value value;
} Variable;

AST *new_ast(AST ast);
AST *new_ast_assign(char *name, AST *value);
AST *new_ast_number(int number);
AST *new_ast_add(AST *left, AST *right);
AST *new_ast_sub(AST *left, AST *right);
AST *new_ast_mul(AST *left, AST *right);
AST *new_ast_div(AST *left, AST *right);

#endif