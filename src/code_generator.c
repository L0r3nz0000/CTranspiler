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
  fprintf(f, "%s(", ast.name);

  for (int i = 0; i < ast.arg_count; i++) {
    generate_code(ast.args[i], f, false, true);

    if (i < ast.arg_count - 1) {
      fprintf(f, ", ");
    }
  }
  fprintf(f, ")");
}

void define_function(AST_FUNCT ast, FILE *f) {
  printf("Function: ");
  if (strcmp(ast.name, "main") == 0) {
    printf("int main(");
  } else {
    printf("Value %s(", ast.name);
  }

  for (int i = 0; i < ast.param_count; i++) {
    printf("Value %s", ast.params[i]);

    if (i < ast.param_count - 1) {
      printf(", ");
    }
  }
  printf(");\n");

  if (strcmp(ast.name, "main") == 0) {
    fprintf(f, "int main(");
  } else {
    fprintf(f, "Value %s(", ast.name);  // return type
  }

  for (int i = 0; i < ast.param_count; i++) {
    fprintf(f, "Value %s", ast.params[i]);  // args type

    if (i < ast.param_count - 1) {
      fprintf(f, ", ");
    }
  }
  fprintf(f, ") {\n");
  
  // Genera il body della funzione
  for (int i = 0; i < ast.body->count; i++) {
    generate_code(ast.body->statements[i], f, strcmp(ast.name, "main") == 0, false);
  }

  fprintf(f, "}\n");
}

void generate_code(AST *ast, FILE *f, bool main, bool nested_call) {
  switch (ast->tag) {
    case TAG_SYSTEM_IMPORT: {
      AST_SYSTEM_IMPORT data = ast->data.ast_system_import;
      fprintf(f, "#include <%s>\n", data.filename);
      return;
    }
    case TAG_IF: {
      AST_IF data = ast->data.ast_if;
      fprintf(f, "if (");
      generate_code(data.condition, f, false, false);
      fprintf(f, ") {\n");

      for (int i = 0; i < data.body->count; i++) {
        generate_code(data.body->statements[i], f, false, false);
      }

      fprintf(f, "}\n");
      return;
    }
    case TAG_WHILE: {
      AST_WHILE data = ast->data.ast_while;
      fprintf(f, "while (");
      generate_code(data.condition, f, false, false);
      fprintf(f, ") {\n");

      for (int i = 0; i < data.body->count; i++) {
        generate_code(data.body->statements[i], f, false, false);
      }

      fprintf(f, "}\n");
      return;
    }
    case TAG_FOR: {
      AST_FOR data = ast->data.ast_for;
      generate_code(data.init, f, false, false);
      fprintf(f, "while (");
      generate_code(data.condition, f, false, false);
      fprintf(f, ") {\n");

      for (int i = 0; i < data.body->count; i++) {
        generate_code(data.body->statements[i], f, false, false);
      }

      if (data.init->tag == TAG_ASSIGN) {
        fprintf(f, "%s = add_operator(%s, ", data.init->data.ast_assign.name, data.init->data.ast_assign.name);
      } else if (data.init->tag == TAG_DECLARE) {
        fprintf(f, "%s = add_operator(%s, ", data.init->data.ast_declare.name, data.init->data.ast_declare.name);
      }
      generate_code(data.step, f, false, false);
      fprintf(f, ");\n");

      fprintf(f, "}\n");
      return;
    }
    case TAG_CONDITION: {
      AST_CONDITION data = ast->data.ast_condition;

      switch (data.type) {
        case TOKEN_EQUALS:
          fprintf(f, "is_equal(");
          break;
        case TOKEN_NOT_EQUALS:
          fprintf(f, "is_not_equal(");
          break;
        case TOKEN_LESS:
          fprintf(f, "is_less(");
          break;
        case TOKEN_GREATER:
          fprintf(f, "is_greater(");
          break;
        case TOKEN_LESS_EQUALS:
          fprintf(f, "is_less_or_equal(");
          break;
        case TOKEN_GREATER_EQUALS:
          fprintf(f, "is_greater_or_equal(");
          break;
      }

      generate_code(data.left, f, false, false);
      fprintf(f, ", ");
      generate_code(data.right, f, false, false);
      fprintf(f, ")");
      return;
    }
    case TAG_VAR: {
      AST_VAR data = ast->data.ast_var;
      fprintf(f, "%s", data.name);
      return;
    }
    case TAG_INT: {
      AST_INT data = ast->data.ast_int;
      fprintf(f, "((Value) {INT, .value.i64val = %d, 0})", data.number);
      return;
    }
    case TAG_FLOAT: {
      AST_FLOAT data = ast->data.ast_float;
      fprintf(f, "((Value) {FLOAT, .value.fval = %f, 0})", data.number);
      return;
    }
    case TAG_STRING: {
      AST_STRING data = ast->data.ast_string;
      fprintf(f, "((Value) {STRING, .value.sval = {\"%s\", %d}, 0})", data.string, strlen(data.string));
      return;
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

      if (main) {
        fprintf(f, "return 0;\n");
      } else {
        fprintf(f, "return ");
        generate_code(data.value, f, false, false);
        fprintf(f, ";\n");
      }
      return;
    }
    case TAG_ASSIGN: {
      AST_ASSIGN data = ast->data.ast_assign;
      fprintf(f, "%s = ", data.name);
      generate_code(data.value, f, false, false);
      fprintf(f, ";\n");
      return;
    }
    case TAG_DECLARE: {
      AST_DECLARE data = ast->data.ast_declare;

      fprintf(f, "Value %s = ", data.name);

      generate_code(data.value, f, false, false);
      fprintf(f, ";\n");
      return;
    }
    case TAG_ADD: {
      AST_ADD data = ast->data.ast_add;
      fprintf(f, "add_operator(");
      generate_code(data.left, f, false, false);
      fprintf(f, ", ");
      generate_code(data.right, f, false, false);
      fprintf(f, ")");
      return;
    }
    case TAG_SUB: {
      AST_SUB data = ast->data.ast_sub;
      fprintf(f, "sub_operator(");
      generate_code(data.left, f, false, false);
      fprintf(f, ", ");
      generate_code(data.right, f, false, false);
      fprintf(f, ")");
      return;
    }
    case TAG_MUL: {
      AST_MUL data = ast->data.ast_mul;
      fprintf(f, "mul_operator(");
      generate_code(data.left, f, false, false);
      fprintf(f, ", ");
      generate_code(data.right, f, false, false);
      fprintf(f, ")");
      return;
    }
    case TAG_DIV: {
      AST_DIV data = ast->data.ast_div;
      fprintf(f, "div_operator(");
      generate_code(data.left, f, false, false);
      fprintf(f, ", ");
      generate_code(data.right, f, false, false);
      fprintf(f, ")");
      return;
    }
  }
}

void generate_c_code(PROGRAM *program, FILE *f) {
  FILE *src = fopen("src/mystdlib.h", "r");  // Carica la libreria standard

  if (!src) {
    perror("Errore nell'apertura del file");
    exit(1);
  }

  int ch;
  while ((ch = fgetc(src)) != EOF) {  // Legge un carattere
    fputc(ch, f);                     // Scrive il carattere nel file di output
  }

  fclose(src);

  for (int i = 0; i < program->block->count; i++) {
    AST *ast = program->block->statements[i];
    generate_code(ast, f, false, false);
  }
}