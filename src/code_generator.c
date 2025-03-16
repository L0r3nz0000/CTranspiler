#include "code_generator.h"

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

void write_method_signature(AST_METHOD ast, FILE *f, bool constructor) {
  int size = strlen(ast.name) + strlen(ast.class_name) + 1;  // +1 per '_'
  char *method_name = malloc(sizeof(char) * (size + 1));  // +1 per '\0'
  snprintf(method_name, size+1, "%s_%s", ast.class_name, ast.name);

  if (constructor) {
    fprintf(f, "Value %s(", method_name);
    printf("Value %s(", method_name);
  } else {
    fprintf(f, "Value %s(Value this", method_name);  // return type + name
    printf("Value %s(Value this", method_name);

    if (ast.param_count > 0) {
      fprintf(f, ", ");
      printf(", ");
    }
  }

  if (ast.param_count > 0) {
    fprintf(f, "Value *args)");
    printf("Value *args);\n");
  } else {
    fprintf(f, ")");
    printf(");\n");
  }
  free(method_name);
}

void define_method(AST_METHOD ast, FILE *f) {
  write_method_signature(ast, f, false);
  fprintf(f, "{\n");

  // Genera le variabili locali passate come parametro
  for (int i = 0; i < ast.param_count; i++) {
    fprintf(f, "Value %s = args[%d];\n", ast.params[i], i);
  }

  // Genera il body del metodo
  for (int i = 0; i < ast.body->count; i++) {
    generate_code(ast.body->statements[i], f, false, false);
  }

  fprintf(f, "}\n");
}

void define_class_constructor(AST_CLASS class, AST_METHOD constructor, FILE *f) {
  write_method_signature(constructor, f, true);
  fprintf(f, "{\n");

  fprintf(f, "\t%s *obj = malloc(sizeof(%s));\n", constructor.class_name, constructor.class_name);
  fprintf(f, "if (obj == NULL) {\n");
  fprintf(f, "\tperror(\"Errore malloc\");\n");
  fprintf(f, "\texit(1);\n");
  fprintf(f, "}\n");
  fprintf(f, "obj->_free_ = %s__free_;\n", constructor.class_name);
  fprintf(f, "obj->_to_string_ = %s__to_string_;\n", constructor.class_name);
  fprintf(f, "obj->class_name = \"%s\";\n", constructor.class_name);
  fprintf(f, "\n");

  fprintf(f, "register_methods(obj, (MethodEntry[]) {\n");
  for (int i = 0; i < class.method_count; i++) {
    fprintf(f, "\t{\"%s\", %s_%s},\n", class.methods[i]->data.ast_method.name, class.name, class.methods[i]->data.ast_method.name);
  }
  fprintf(f, "}, %d);\n", class.method_count);

  fprintf(f, "register_fields(obj, (FieldEntry[]) {\n");
  for (int i = 0; i < class.field_count; i++) {
    fprintf(f, "\t{\"%s\", NONE},\n", class.fields[i]->data.ast_field.name);
  }
  fprintf(f, "}, %d);\n\n", class.field_count);
  fprintf(f, "Value this = toObject(obj);\n");

  // Genera le variabili locali passate come parametro
  for (int i = 0; i < constructor.param_count; i++) {
    fprintf(f, "Value %s = args[%d];\n", constructor.params[i], i);
  }

  // Genera il body del costruttore
  for (int i = 0; i < constructor.body->count; i++) {
    generate_code(constructor.body->statements[i], f, false, false);
  }

  fprintf(f, "return this;\n");
  fprintf(f, "}\n");
}

void define_class_destructor(AST_CLASS ast, FILE *f) {
  fprintf(f, "void %s__free_(Value this) {\n", ast.name);
  fprintf(f, "if (this.value.obj_ptr == NULL) {\n\tprintf(\"Freeing null object\\n\"); \n} else {\n");
  fprintf(f, "\tfree(this.value.obj_ptr);\n}");
  fprintf(f, "}\n");
}

void define_class(AST_CLASS ast, FILE *f) {
  char *name = ast.name;
  AST **methods = ast.methods;
  int method_count = ast.method_count;
  
  fprintf(f, "typedef Object %s;\n", name);

  // Scrive le firme dei metodi
  bool to_string_defined = false;
  fprintf(f, "// Metodi della classe %s\n", name);
  for (int i = 0; i < method_count; i++) {
    if (strcmp(methods[i]->data.ast_method.name, "_to_string_") == 0) to_string_defined = true;
    write_method_signature(methods[i]->data.ast_method, f, strcmp(methods[i]->data.ast_method.name, "_new_") == 0);
    fprintf(f, ";\n");
  }

  if (!to_string_defined) {
    fprintf(f, "Value %s__to_string_(Value this) {\n", name);
    fprintf(f, "\treturn toString(this.value.obj_ptr->class_name);\n");
    fprintf(f, "}\n");
  }

  // Definisce il distruttore
  define_class_destructor(ast, f);

  // Definisce gli altri metodi di classe
  for (int i = 0; i < method_count; i++) {
    if (strcmp(methods[i]->data.ast_method.name, "_new_") == 0) {
      define_class_constructor(ast, methods[i]->data.ast_method, f);
    } else {
      define_method(methods[i]->data.ast_method, f);
    }
  }
}

void generate_code(AST *ast, FILE *f, bool main, bool nested_call) {
  switch (ast->tag) {
    case TAG_IF: {
      AST_IF data = ast->data.ast_if;
      fprintf(f, "if (c_bool(");
      generate_code(data.condition, f, false, true);
      fprintf(f, ")) {\n");

      for (int i = 0; i < data.body->count; i++) {
        generate_code(data.body->statements[i], f, false, false);
      }

      fprintf(f, "}\n");
      break;
    }
    case TAG_CLASS: {
      AST_CLASS data = ast->data.ast_class;

      define_class(data, f);
      break;
    }
    case TAG_SYSTEM_IMPORT: {
      AST_SYSTEM_IMPORT data = ast->data.ast_system_import;
      fprintf(f, "#include <%s>\n", data.filename);
      break;
    }
    case TAG_WHILE: {
      AST_WHILE data = ast->data.ast_while;
      fprintf(f, "while (c_bool(");
      generate_code(data.condition, f, false, true);
      fprintf(f, ")) {\n");

      for (int i = 0; i < data.body->count; i++) {
        generate_code(data.body->statements[i], f, false, false);
      }

      fprintf(f, "}\n");
      break;
    }
    case TAG_FOR: {
      AST_FOR data = ast->data.ast_for;
      generate_code(data.init, f, false, false);
      fprintf(f, "while (c_bool(");
      generate_code(data.condition, f, false, true);
      fprintf(f, ")) {\n");

      // Body del for
      for (int i = 0; i < data.body->count; i++) {
        generate_code(data.body->statements[i], f, false, false);
      }

      // Incremento 
      if (data.init->tag == TAG_ASSIGN) {
        fprintf(f, "%s = add_operator(%s, ", data.init->data.ast_assign.name, data.init->data.ast_assign.name);
      } else if (data.init->tag == TAG_DECLARE) {
        fprintf(f, "%s = add_operator(%s, ", data.init->data.ast_declare.name, data.init->data.ast_declare.name);
      }
      generate_code(data.step, f, false, true);
      fprintf(f, ");\n");

      fprintf(f, "}\n");
      break;
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

      generate_code(data.left, f, false, true);
      fprintf(f, ", ");
      generate_code(data.right, f, false, true);
      fprintf(f, ")");
      break;
    }
    case TAG_VAR: {
      AST_VAR data = ast->data.ast_var;
      fprintf(f, "%s", data.name);
      break;
    }
    case TAG_FIELD_ACCESS: { // get field
      AST_FIELD_ACCESS data = ast->data.ast_field_access;
      fprintf(f, "get_field(%s.value.obj_ptr, \"%s\")", data.name, data.field);
      if (!nested_call) {
        fprintf(f, ";\n");
      }
      break;
    }
    case TAG_INT: {
      AST_INT data = ast->data.ast_int;
      fprintf(f, "((Value) {INT, .value.ival = %d})", data.number);
      break;
    }
    case TAG_FLOAT: {
      AST_FLOAT data = ast->data.ast_float;
      fprintf(f, "((Value) {FLOAT, .value.fval = %f})", data.number);
      break;
    }
    case TAG_STRING: {
      AST_STRING data = ast->data.ast_string;
      fprintf(f, "((Value) {STRING, .value.sval = {\"%s\", %d}})", data.string, strlen(data.string));
      break;
    }
    case TAG_FUN: {
      AST_FUNCT data = ast->data.ast_funct;
      define_function(data, f);
      break;
    }
    case TAG_CALL: {
      AST_CALL data = ast->data.ast_call;
      
      generate_function_call(data, f);
      if (!nested_call) {
        fprintf(f, ";\n");
      }
      break;
    }
    case TAG_RETURN: {
      AST_RETURN data = ast->data.ast_return;

      if (main) {
        fprintf(f, "return c_int(");
        generate_code(data.value, f, false, true);
        fprintf(f, ");\n");
      } else {
        fprintf(f, "return ");
        generate_code(data.value, f, false, true);
        fprintf(f, ";\n");
      }
      break;
    }
    case TAG_ASSIGN: {
      AST_ASSIGN data = ast->data.ast_assign;

      if (data.name->tag == TAG_FIELD_ACCESS) {
        AST_FIELD_ACCESS field_data = data.name->data.ast_field_access;
        fprintf(f, "set_field(%s.value.obj_ptr, \"%s\", ", field_data.name, field_data.field);
        generate_code(data.value, f, false, true);
        fprintf(f, ");\n");
      } else {
        generate_code(data.name, f, false, true);
        fprintf(f, " = ");
        generate_code(data.value, f, false, true);
        fprintf(f, ";\n");
      }
      break;
    }
    case TAG_DECLARE: {
      AST_DECLARE data = ast->data.ast_declare;

      fprintf(f, "Value ");
      generate_code(data.name, f, false, true);
      fprintf(f, " = ");

      generate_code(data.value, f, false, true);
      fprintf(f, ";\n");
      break;
    }
    case TAG_ADD: {
      AST_ADD data = ast->data.ast_add;
      fprintf(f, "add_operator(");
      generate_code(data.left, f, false, true);
      fprintf(f, ", ");
      generate_code(data.right, f, false, true);
      fprintf(f, ")");
      break;
    }
    case TAG_SUB: {
      AST_SUB data = ast->data.ast_sub;
      fprintf(f, "sub_operator(");
      generate_code(data.left, f, false, true);
      fprintf(f, ", ");
      generate_code(data.right, f, false, true);
      fprintf(f, ")");
      break;
    }
    case TAG_MUL: {
      AST_MUL data = ast->data.ast_mul;
      fprintf(f, "mul_operator(");
      generate_code(data.left, f, false, true);
      fprintf(f, ", ");
      generate_code(data.right, f, false, true);
      fprintf(f, ")");
      break;
    }
    case TAG_DIV: {
      AST_DIV data = ast->data.ast_div;
      fprintf(f, "div_operator(");
      generate_code(data.left, f, false, true);
      fprintf(f, ", ");
      generate_code(data.right, f, false, true);
      fprintf(f, ")");
      break;
    }
    case TAG_MOD: {
      AST_MOD data = ast->data.ast_mod;
      fprintf(f, "mod_operator(");
      generate_code(data.left, f, false, true);
      fprintf(f, ", ");
      generate_code(data.right, f, false, true);
      fprintf(f, ")");
      break;
    }
  }
}

void generate_c_code(AST_BLOCK *block, FILE *f) {
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

  // Dichiarazioni globali e import
  for (int i = 0; i < block->count; i++) {
    AST *ast = block->statements[i];
    if (ast->tag == TAG_SYSTEM_IMPORT || ast->tag == TAG_DECLARE) {
      generate_code(ast, f, true, false);
    }
  }

  // Scrive il codice delle classi
  for (int i = 0; i < block->count; i++) {
    AST *ast = block->statements[i];
    if (ast->tag == TAG_CLASS) {
      generate_code(ast, f, false, false);
    }
  }

  // Scrive il codice delle funzioni
  for (int i = 0; i < block->count; i++) {
    AST *ast = block->statements[i];
    if (ast->tag == TAG_FUN) {
      generate_code(ast, f, false, false);
    }
  }
}