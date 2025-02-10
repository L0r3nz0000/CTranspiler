#pragma once
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H
#include "parser.h"   // AST, Variable
#include <stdio.h>

void generate_asm(AST_BLOCK *block, FILE* f);

#endif