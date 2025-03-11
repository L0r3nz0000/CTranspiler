#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lexer.h"  // Value, TokenType

typedef int bool;
#define true 1
#define false 0

typedef struct AST AST;

typedef enum {
  CHAR,
  INT,
  FLOAT,
  STRING,
  FUNCTION,
  VOID
} VarType;

typedef struct {
  char *name;
  VarType type;
  Value value;
} Variable;

typedef struct {
  Variable *vars;
  int count;
} SYMBOL_TABLE;

typedef struct {
  AST **statements;  // Array of AST*
  int count;         // Number of instructions
} AST_BLOCK;

typedef struct {
  AST_BLOCK *block;
  SYMBOL_TABLE *symbol_table;
} PROGRAM;

typedef struct { VarType type; char *name; } AST_VAR;
typedef struct { char* name; AST *value; } AST_ASSIGN;  // TODO: usare AST_VAR per il nome

typedef struct { int32_t number; } AST_INT;
typedef struct { float number; } AST_FLOAT;
typedef struct { char *string; } AST_STRING;

typedef struct { char *filename; } AST_SYSTEM_IMPORT;

typedef struct { AST *left; AST *right; TokenType type; } AST_CONDITION;
typedef struct { AST *condition; AST_BLOCK *body; } AST_IF;
typedef AST_IF AST_WHILE;
typedef struct { AST *init; AST *condition; AST *step; AST_BLOCK *body; } AST_FOR;

// Operatori binari
typedef struct { VarType ret_type; AST *left; AST *right; } BINARY_OP;
typedef BINARY_OP AST_ADD;
typedef BINARY_OP AST_SUB;
typedef BINARY_OP AST_MUL;
typedef BINARY_OP AST_DIV;
typedef BINARY_OP AST_MOD;

typedef struct {
  VarType ret_type;       // Return type
  char *name;             // Function name
  VarType *param_types;   // Parameter types
  char **params;          // Parameter list
  int param_count;        // Number of parameters
  AST_BLOCK *body;        // Body (array of instructions)
} AST_FUNCT;

typedef struct { AST *value; } AST_RETURN;
typedef struct { VarType type; char *name; AST *value; } AST_DECLARE;

typedef struct {
  VarType ret_type;   // Return type
  char *name;         // Name of the called function
  VarType *arg_types; // Types of the arguments
  AST **args;         // Argument list
  int arg_count;      // Number of arguments
} AST_CALL;

typedef enum {
  TAG_ASSIGN,
  TAG_INT,
  TAG_FLOAT,
  TAG_ADD,
  TAG_SUB,
  TAG_MUL,
  TAG_DIV,
  TAG_MOD,
  TAG_STRING,
  TAG_FUN,
  TAG_CALL,
  TAG_RETURN,
  TAG_BLOCK,
  TAG_DECLARE,
  TAG_VAR,
  TAG_SYSTEM_IMPORT,
  TAG_IF,
  TAG_WHILE,
  TAG_FOR,
  TAG_CONDITION,
} AST_TAG;

typedef struct AST {
  AST_TAG tag;

  union {
    AST_ASSIGN ast_assign;
    AST_INT ast_int;
    AST_FLOAT ast_float;
    AST_STRING ast_string;
    AST_VAR ast_var;
    AST_FUNCT ast_funct;
    AST_CALL ast_call;
    AST_RETURN ast_return;
    AST_DECLARE ast_declare;
    AST_SYSTEM_IMPORT ast_system_import;
    AST_IF ast_if;
    AST_CONDITION ast_condition;
    AST_WHILE ast_while;
    AST_FOR ast_for;

    // Operatori binari
    AST_ADD ast_add;
    AST_SUB ast_sub;
    AST_MUL ast_mul;
    AST_DIV ast_div;
    AST_MOD ast_mod;
  } data;
} AST;

AST *new_ast(AST ast);
AST *new_ast_assign(char *name, AST *value);
AST *new_ast_int64(int64_t number);
AST *new_ast_float(float number);
AST *new_ast_add(VarType ret_type, AST *left, AST *right);
AST *new_ast_sub(VarType ret_type, AST *left, AST *right);
AST *new_ast_mul(VarType ret_type, AST *left, AST *right);
AST *new_ast_div(VarType ret_type, AST *left, AST *right);
AST *new_ast_funct(VarType ret_type, char *name, VarType *param_types, char **params, int param_count, AST_BLOCK *body);
AST *new_ast_declare(VarType type, char *name, AST *value);
AST *new_ast_var(VarType type, char *name);

VarType typeOf(AST* ast);  // Returns the data type of something
AST *generate_tree(TokenList tl, bool parse_functions);
void print_tree(AST *node, int indent);
void *parse_program(TokenList tl, bool parse_functions);

#endif