// Start mysdtlib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  CHAR,
  INT,
  FLOAT,
  STRING,
} DataType;

typedef struct {
  char *str;
  int size;
} string;

typedef struct {
  DataType tag;
  union {
    char cval;    // char
    long i64val;  // int
    float fval;   // float
    string sval;  // string
  } value;
  int is_none;
} Value;

#define NONE (Variable) {0, {0}, 1}

char *typeOf(Value v) {
  switch (v.tag) {
    case CHAR: return "char";
    case INT: return "int";
    case FLOAT: return "float";
    case STRING: return "string";
    default: return "unknown";
  }
}

Value type(Value v) {
  return (Value) {STRING, .value.sval = {typeOf(v), strlen(typeOf(v))}, 0};
}

Value add_operator(Value a, Value b) {  // Operatore +
  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.i64val = a.value.i64val + b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval + b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.i64val = a.value.i64val + b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.i64val = a.value.fval + b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '+' operator. Cannot add '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value sub_operator(Value a, Value b) {  // Operatore -
  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.i64val = a.value.i64val - b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval - b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.i64val = a.value.i64val - b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.i64val = a.value.fval - b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '-' operator. Cannot add '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value mul_operator(Value a, Value b) {  // Operatore *
  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.i64val = a.value.i64val * b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval * b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.i64val = a.value.i64val * b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.i64val = a.value.fval * b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '*' operator. Cannot add '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value div_operator(Value a, Value b) {  // Operatore /
  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.i64val = a.value.i64val / b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval / b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.i64val = a.value.i64val / b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.i64val = a.value.fval / b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '/' operator. Cannot add '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value toInt(Value v) {
  if (v.tag == INT) {
    return v;
  } else if (v.tag == FLOAT) {
    return (Value) {INT, .value.i64val = (int)v.value.fval, 0};
  } else if (v.tag == STRING) {
    long num = atoi(v.value.sval.str);
    if (num == 0) {
      printf("Error: Cannot convert '%s' to int.\n", v.value.sval.str);
      exit(1);
    }
    return (Value) {INT, .value.i64val = num, 0};
  } else if (v.tag == CHAR) {
    if (v.value.cval >= '0' && v.value.cval <= '9') {
      return (Value) {INT, .value.i64val = v.value.cval - '0', 0};
    } else {
      printf("Error: Cannot convert '%c' to int.\n", v.value.cval);
      exit(1);
    }
  } else {
    printf("Error: Cannot convert '%s' to int.\n", typeOf(v));
    exit(1);
  }
}

void print(Value data) {
  if (data.is_none) {
    printf("None\n");
    return;
  } else if (data.tag == INT) {
    printf("%d", data.value.i64val);
  } else if (data.tag == FLOAT) {
    printf("%f", data.value.fval);
  } else if (data.tag == CHAR) {
    printf("%c", data.value.cval);
  } else if (data.tag == STRING) {
    printf("%s", data.value.sval.str);
  }
}

void println(Value data) {
  print(data);
  printf("\n");
}
// End mysdtlib
Value funzione(Value x) {
return add_operator(x, (Value) {INT, .value.i64val = 2, 0});
}
