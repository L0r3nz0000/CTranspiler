#include "code_generator.h"

void write_type(VarType t, FILE *f) {
  switch (t) {
    case CHAR:
      fprintf(f, "char");
      break;
    case INT:
      fprintf(f, "int");
      break;
    case FLOAT:
      fprintf(f, "float");
      break;
    case STRING:
      fprintf(f, "string");
      break;
    case NONE_TYPE:
      fprintf(f, "none");
      break;
  }
}

void print_type(VarType t) {
  write_type(t, stdout);
}

void generate_function_call(AST_CALL ast, FILE *f) {
  fprintf(f, "%s(", ast.name);

  for (int i = 0; i < ast.arg_count; i++) {
    fprintf(f, " %s", ast.args[i]);

    if (i < ast.arg_count - 1) {
      fprintf(f, ", ");
    }
  }
  fprintf(f, ")\n");
}

void define_function(AST_FUNCT ast, FILE *f) {
  printf("Defining: ");
  print_type(ast.ret_type);
  printf(" %s(", ast.name);

  for (int i = 0; i < ast.param_count; i++) {
    print_type(INT);//print_type(ast.param_types[i]);
    printf(" %s", ast.params[i]);

    if (i < ast.param_count - 1) {
      printf(", ");
    }
  }
  printf(");\n");

  write_type(ast.ret_type, f);
  fprintf(f, " %s(", ast.name);

  for (int i = 0; i < ast.param_count; i++) {
    write_type(INT, f);//write_type(ast.arg_types[i]);

    fprintf(f, " %s", ast.params[i]);

    if (i < ast.param_count - 1) {
      fprintf(f, ", ");
    }
  }
  fprintf(f, ") {\n");
  generate_code(ast.body, f);
  fprintf(f, "}\n");
}

void generate_code(AST_BLOCK *ptr, FILE *f) {
  for (int i = 0; i < ptr->count; i++) {
    AST ast = *ptr->statements[i];

    switch (ast.tag) {
      case TAG_VAR: {
        AST_VAR data = ast.data.ast_var;
        fprintf("%s", data.name);
        continue;
      }
      case TAG_INT: {
        AST_INT data = ast.data.ast_int;
        fprintf(f, "%d", data.number);
        continue;
      }
      case TAG_FLOAT: {
        AST_FLOAT data = ast.data.ast_float;
        fprintf(f, "%f", data.number);
      }
      case TAG_FUN: {
        AST_FUNCT data = ast.data.ast_funct;
        define_function(data, f);
        continue;
      }
      case TAG_CALL: {
        AST_CALL data = ast.data.ast_call;
        generate_function_call(data, f);
        continue;
      }

      case TAG_RETURN: {
        AST_RETURN data = ast.data.ast_return;
        
        fprintf(f, "return ");
        generate_code(&(AST_BLOCK){&data.value, 1}, f);
        fprintf(f, ";\n");
        continue;
      }
      case TAG_ASSIGN: {
        AST_ASSIGN data = ast.data.ast_assign;
        fprintf(f, "%s = ", data.name);
        generate_code(&(AST_BLOCK){&data.value, 1}, f);
        fprintf(f, ";\n");
        continue;
      }
      case TAG_DECLARE: {
        AST_DECLARE data = ast.data.ast_declare;
        write_type(data.type, f);
        fprintf(f, " %s = ", data.name);
        generate_code(&(AST_BLOCK){&data.value, 1}, f);
        fprintf(f, ";\n");
        continue;
      }
      case TAG_ADD: {
        AST_ADD data = ast.data.ast_add;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, " + ");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        continue;
      }
      case TAG_SUB: {
        AST_SUB data = ast.data.ast_sub;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, " - ");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        continue;
      }
      case TAG_MUL: {
        AST_MUL data = ast.data.ast_mul;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, " * ");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        continue;
      }
      case TAG_DIV: {
        AST_DIV data = ast.data.ast_div;
        generate_code(&(AST_BLOCK){&data.left, 1}, f);
        fprintf(f, " / ");
        generate_code(&(AST_BLOCK){&data.right, 1}, f);
        continue;
      }
    }
  }
}

void generate_c_code(PROGRAM *program, FILE *f) {
  generate_code(program->block, f);
}