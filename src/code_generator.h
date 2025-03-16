#pragma once
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H
#include "parser.h"   // AST, Variable
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void generate_c_code(AST_BLOCK *block, FILE* f);
void generate_code(AST *ast, FILE *f, bool main, bool nested_call);

#endif