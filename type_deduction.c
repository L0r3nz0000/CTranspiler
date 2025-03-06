/*

Codice di esempio per la generazione di codice C con type deduction a runtime.

*/

#include <stdio.h>

typedef enum {
  CHAR,
  INT,
  FLOAT,
  STRING,
} DataType;

typedef struct {
  DataType tag;
  union {
    char cval;    // char
    long i64val;  // int
    float fval;   // float
    char *sval;   // string
  } value;
  int is_none;
} Variable;

#define NONE (Variable) {0, {0}, 1}

char *tipeOf(Variable v) {
  switch (v.tag) {
    case CHAR: return "char";
    case INT: return "int";
    case FLOAT: return "float";
    case STRING: return "string";
    default: return "unknown";
  }
}

Variable add(Variable a, Variable b) {  // Operatore +
  if (a.tag == INT && b.tag == INT) {
    return (Variable) {INT, .value.i64val = a.value.i64val + b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.fval = a.value.fval + b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.i64val = a.value.i64val + b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Variable) {FLOAT, .value.i64val = a.value.fval + b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '+' operator. Cannot add '%s' and '%s'.\n", tipeOf(a), tipeOf(b));
    exit(1);
  }
}

Variable sub(Variable a, Variable b) {  // Operatore -
  if (a.tag == INT && b.tag == INT) {
    return (Variable) {INT, .value.i64val = a.value.i64val - b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.fval = a.value.fval - b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.i64val = a.value.i64val - b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Variable) {FLOAT, .value.i64val = a.value.fval - b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '-' operator. Cannot add '%s' and '%s'.\n", tipeOf(a), tipeOf(b));
    exit(1);
  }
}

Variable mul(Variable a, Variable b) {  // Operatore *
  if (a.tag == INT && b.tag == INT) {
    return (Variable) {INT, .value.i64val = a.value.i64val * b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.fval = a.value.fval * b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.i64val = a.value.i64val * b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Variable) {FLOAT, .value.i64val = a.value.fval * b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '*' operator. Cannot add '%s' and '%s'.\n", tipeOf(a), tipeOf(b));
    exit(1);
  }
}

Variable div(Variable a, Variable b) {  // Operatore /
  if (a.tag == INT && b.tag == INT) {
    return (Variable) {INT, .value.i64val = a.value.i64val / b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.fval = a.value.fval / b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Variable) {FLOAT, .value.i64val = a.value.i64val / b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Variable) {FLOAT, .value.i64val = a.value.fval / b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '/' operator. Cannot add '%s' and '%s'.\n", tipeOf(a), tipeOf(b));
    exit(1);
  }
}