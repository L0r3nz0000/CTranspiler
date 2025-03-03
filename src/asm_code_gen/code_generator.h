#pragma once
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H
#include "parser.h"   // AST, Variable
#include <stdio.h>

void generate_asm(PROGRAM *program, FILE* f, bool no_stdlib);

#endif