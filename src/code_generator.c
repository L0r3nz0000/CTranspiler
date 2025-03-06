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
    case VOID:
      fprintf(f, "void");
      break;
  }
}

void print_type(VarType t) {
  write_type(t, stdout);
}

void generate_function_call(AST_CALL ast, FILE *f) {
  // DEBUG
  printf("Calling function: %s\n", ast.name);
  printf("Arguments: %d\n", ast.arg_count);

  fprintf(f, "%s(", ast.name);

  for (int i = 0; i < ast.arg_count; i++) {
    generate_code(ast.args[i], f, true);

    if (i < ast.arg_count - 1) {
      fprintf(f, ", ");
    }
  }
  fprintf(f, ")");
}

void define_function(AST_FUNCT ast, FILE *f) {
  printf("Defining: ");
  print_type(ast.ret_type);
  printf(" %s(", ast.name);

  for (int i = 0; i < ast.param_count; i++) {
    print_type(INT); // TODO: riconoscere automaticamente i tipi di dati     print_type(ast.param_types[i]);
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
  
  // Genera il body della funzione
  for (int i = 0; i < ast.body->count; i++) {
    generate_code(ast.body->statements[i], f, false);
  }

  fprintf(f, "}\n");
}

void generate_code(AST *ast, FILE *f, bool nested_call) {
  switch (ast->tag) {
    case TAG_SYSTEM_IMPORT: {
      AST_SYSTEM_IMPORT data = ast->data.ast_system_import;
      fprintf(f, "#include <%s>\n", data.filename);
      return;
    }
    case TAG_VAR: {
      AST_VAR data = ast->data.ast_var;
      fprintf(f, "%s", data.name);
      return;
    }
    case TAG_INT: {
      AST_INT data = ast->data.ast_int;
      fprintf(f, "%d", data.number);
      return;
    }
    case TAG_FLOAT: {
      AST_FLOAT data = ast->data.ast_float;
      fprintf(f, "%f", data.number);
    }
    case TAG_FUN: {
      AST_FUNCT data = ast->data.ast_funct;
      define_function(data, f);
      return;
    }
    case TAG_CALL: {
      AST_CALL data = ast->data.ast_call;
      generate_function_call(data, f);
      if (!nested_call) {
        fprintf(f, ";\n");
      }
      return;
    }
    case TAG_RETURN: {
      AST_RETURN data = ast->data.ast_return;
      
      fprintf(f, "return ");
      generate_code(data.value, f, false);
      fprintf(f, ";\n");
      return;
    }
    case TAG_ASSIGN: {
      AST_ASSIGN data = ast->data.ast_assign;
      fprintf(f, "%s = ", data.name);
      generate_code(data.value, f, false);
      fprintf(f, ";\n");
      return;
    }
    case TAG_DECLARE: {
      AST_DECLARE data = ast->data.ast_declare;
      write_type(data.type, f);
      fprintf(f, " %s = ", data.name);
      generate_code(data.value, f, false);
      fprintf(f, ";\n");
      return;
    }
    case TAG_ADD: {
      AST_ADD data = ast->data.ast_add;
      generate_code(data.left, f, false);
      fprintf(f, " + ");
      generate_code(data.right, f, false);
      return;
    }
    case TAG_SUB: {
      AST_SUB data = ast->data.ast_sub;
      generate_code(data.left, f, false);
      fprintf(f, " - ");
      generate_code(data.right, f, false);
      return;
    }
    case TAG_MUL: {
      AST_MUL data = ast->data.ast_mul;
      generate_code(data.left, f, false);
      fprintf(f, " * ");
      generate_code(data.right, f, false);
      return;
    }
    case TAG_DIV: {
      AST_DIV data = ast->data.ast_div;
      generate_code(data.left, f, false);
      fprintf(f, " / ");
      generate_code(data.right, f, false);
      return;
    }
  }
}

void generate_c_code(PROGRAM *program, FILE *f) {
  fprintf(f, "#include <stdio.h>\n");
  fprintf(f, "void print_int(int n) { printf(\"%%d\\n\", n); }\n");
  fprintf(f, "void print_float(float n) { printf(\"%%f\\n\", n); }\n");
  fprintf(f, "void print_char(char n) { printf(\"%%c\\n\", n); }\n");

  for (int i = 0; i < program->block->count; i++) {
    AST *ast = program->block->statements[i];
    generate_code(ast, f, false);
  }
}