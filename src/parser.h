#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lexer.h"  // Value, TokenType

typedef struct AST AST;

typedef struct { char *name; AST *value; } AST_ASSIGN;
typedef struct { int32_t number; } AST_INT;
typedef struct { float number; } AST_FLOAT;
typedef struct { char *string; } AST_STRING;

typedef struct { AST *left; AST *right; } AST_ADD;
typedef struct { AST *left; AST *right; } AST_SUB;
typedef struct { AST *left; AST *right; } AST_MUL;
typedef struct { AST *left; AST *right; } AST_DIV;

typedef enum {
  TAG_ASSIGN,
  TAG_INT,
  TAG_FLOAT,
  TAG_ADD,
  TAG_SUB,
  TAG_MUL,
  TAG_DIV,
  TAG_STRING,
} AST_TAG;

typedef struct AST {
  AST_TAG tag;

  union {
    AST_ASSIGN ast_assign;
    AST_INT ast_int;
    AST_FLOAT ast_float;
    AST_STRING ast_string;
    
    AST_ADD ast_add;
    AST_SUB ast_sub;
    AST_MUL ast_mul;
    AST_DIV ast_div;
  } data;
} AST;

typedef enum {
  CHAR,
  INT,
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
AST *new_ast_int64(int64_t number);
AST *new_ast_float(float number);
AST *new_ast_add(AST *left, AST *right);
AST *new_ast_sub(AST *left, AST *right);
AST *new_ast_mul(AST *left, AST *right);
AST *new_ast_div(AST *left, AST *right);
AST *generate_tree(TokenList tl);
void print_tree(AST *node, int indent);

#endif