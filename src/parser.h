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
typedef struct { AST* name; AST *value; } AST_ASSIGN;

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
  char *name;             // Function name
  char **params;          // Parameter list
  int param_count;        // Number of parameters
  AST_BLOCK *body;        // Body (array of instructions)
} AST_FUNCT;

typedef struct {
  char *class_name;
  char *name;     
  char **params;  
  int param_count;
  AST_BLOCK *body;
} AST_METHOD;

typedef struct { char *name; } AST_FIELD;
typedef struct { char *name; char *field; } AST_FIELD_ACCESS;

typedef struct {
  char *name; 
  AST **fields;
  int field_count;

  AST **methods;
  int method_count;
} AST_CLASS;

typedef struct { AST *value; } AST_RETURN;
typedef struct { VarType type; AST *name; AST *value; } AST_DECLARE;

typedef struct {
  char *name;         // Name of the called function
  AST **args;         // Argument list
  int arg_count;      // Number of arguments
} AST_CALL;

typedef struct {
  char *class_name;
  char *name;   
  AST **args;   
  int arg_count;
} AST_METHOD_CALL;

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
  TAG_CLASS,
  TAG_METHOD,
  TAG_FIELD,
  TAG_FIELD_ACCESS,
  TAG_METHOD_CALL,
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
    AST_CLASS ast_class;
    AST_METHOD ast_method;
    AST_FIELD ast_field;
    AST_FIELD_ACCESS ast_field_access;
    AST_METHOD_CALL ast_method_call;

    // Operatori binari
    AST_ADD ast_add;
    AST_SUB ast_sub;
    AST_MUL ast_mul;
    AST_DIV ast_div;
    AST_MOD ast_mod;
  } data;
} AST;

AST *new_ast(AST ast);
AST *new_ast_assign(AST *name, AST *value);
AST *new_ast_int64(int64_t number);
AST *new_ast_float(double number);
AST *new_ast_add(AST *left, AST *right);
AST *new_ast_sub(AST *left, AST *right);
AST *new_ast_mul(AST *left, AST *right);
AST *new_ast_div(AST *left, AST *right);
AST *new_ast_funct(char *name, char **params, int param_count, AST_BLOCK *body);
AST *new_ast_declare(AST *name, AST *value);
AST *new_ast_var(char *name);

VarType typeOf(AST* ast);  // Returns the data type of something
AST *generate_tree(TokenList tl);
void print_tree(AST *node, int indent);
AST_BLOCK *parse_program(TokenList tl, bool parse_functions, bool parse_classes);
AST_BLOCK *define_all_functions(TokenList tl, int max_depth);

#endif