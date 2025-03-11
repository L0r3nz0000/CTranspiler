// Start mysdtlib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
  CHAR,
  INT,
  FLOAT,
  STRING,
  BOOL,
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
    bool bval;    // bool
  } value;
  int is_none;
} Value;

void print(Value data);

#define NONE (Value) {0, {0}, 1}

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

Value concat_strings(Value string1, Value string2) {
  if (string1.tag != STRING || string2.tag != STRING) {
    printf("Error: Invalid data types for concatenation. Cannot concatenate '%s' and '%s'.\n", typeOf(string1), typeOf(string2));
    exit(1);
  }

  int size = string1.value.sval.size + string2.value.sval.size;
  char *str = (char *) malloc(size + 1);
  strncpy(str, string1.value.sval.str, string1.value.sval.size);
  strncpy(str + string1.value.sval.size, string2.value.sval.str, string2.value.sval.size);
  str[size] = '\0';

  return (Value) {STRING, .value.sval = {str, size}, 0};
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
  } else if (a.tag == STRING && b.tag == STRING) {
    return concat_strings(a, b);
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
    printf("Error: Invalid data types for '-' operator. Cannot subtract '%s' and '%s'.\n", typeOf(a), typeOf(b));
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
  } else if (a.tag == STRING && b.tag == INT) {
    if (b.value.i64val < 0) {
      printf("Error: Cannot multiply string by negative number.\n");
      exit(1);
    }
    Value result = {STRING, .value.sval = {"", 0}, 0};

    for (int i = 0; i < b.value.i64val; i++) {
      result = concat_strings(result, a);
    }
    return result;
  } else if (a.tag == INT && b.tag == STRING) {
    if (a.value.i64val < 0) {
      printf("Error: Cannot multiply string by negative number.\n");
      exit(1);
    }
    Value result = {STRING, .value.sval = {"", 0}, 0};

    for (int i = 0; i < a.value.i64val; i++) {
      result = concat_strings(result, b);
    }
    return result;
  } else {
    printf("Error: Invalid data types for '*' operator. Cannot multiply '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value div_operator(Value a, Value b) {  // Operatore /
  if ((b.tag == INT && b.value.i64val == 0) || (b.tag == FLOAT && b.value.fval == 0)) {
    printf("Error: Zero division error (in module operation) Cannot divide '");
    print(a);
    printf("' by zero.\n");
    exit(1);
  }


  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.i64val = a.value.i64val / b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval / b.value.fval, 0};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.i64val = a.value.i64val / b.value.fval, 0};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.i64val = a.value.fval / b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '/' operator. Cannot divide '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value mod_operator(Value a, Value b) { // Operatore %
  if (b.tag == INT && b.value.i64val == 0) {
    printf("Error: Zero division error (in module operation) Cannot divide '");
    print(a);
    printf("' by zero.\n");
    exit(1);
  }

  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.i64val = a.value.i64val % b.value.i64val, 0};
  } else {
    printf("Error: Invalid data types for '%' operator. Cannot divide '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value is_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.i64val == b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval == b.value.fval, 0};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval == b.value.i64val, 0};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.i64val == b.value.fval, 0};
  } else if (a.tag == STRING && b.tag == STRING) {
    return (Value) {BOOL, .value.bval = strcmp(a.value.sval.str, b.value.sval.str) == 0, 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval == b.value.cval, 0};
  } else {
    printf("Error: Invalid data types for '==' operator. Cannot compare '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value is_not_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.i64val != b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval != b.value.fval, 0};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval != b.value.i64val, 0};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.i64val != b.value.fval, 0};
  } else if (a.tag == STRING && b.tag == STRING) {
    return (Value) {BOOL, .value.bval = strcmp(a.value.sval.str, b.value.sval.str) != 0, 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval != b.value.cval, 0};
  } else {
    printf("Error: Invalid data types for '!=' operator. Cannot compare '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value is_less(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.i64val < b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval < b.value.fval, 0};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval < b.value.i64val, 0};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.i64val < b.value.fval, 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval < b.value.cval, 0};
  } else {
    printf("Error: Invalid data types for '<' operator. Cannot compare '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value is_greater(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.i64val > b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval > b.value.fval, 0};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval > b.value.i64val, 0};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.i64val > b.value.fval, 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval > b.value.cval, 0};
  } else {
    printf("Error: Invalid data types for '>' operator. Cannot compare '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value is_less_or_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.i64val <= b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval <= b.value.fval, 0};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval <= b.value.i64val, 0};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.i64val <= b.value.fval, 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval <= b.value.cval, 0};
  } else {
    printf("Error: Invalid data types for '<=' operator. Cannot compare '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value is_greater_or_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.i64val >= b.value.i64val, 0};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval >= b.value.fval, 0};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval >= b.value.i64val, 0};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.i64val >= b.value.fval, 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval >= b.value.cval, 0};
  } else {
    printf("Error: Invalid data types for '>=' operator. Cannot compare '%s' and '%s'.\n", typeOf(a), typeOf(b));
    exit(1);
  }
}

Value toBool(Value v) {
  if (v.tag == BOOL) {
    return v;
  } else if (v.tag == INT) {
    return (Value) {BOOL, .value.bval = v.value.i64val != 0, 0};
  } else if (v.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = v.value.fval != 0, 0};
  } else if (v.tag == STRING) {
    return (Value) {BOOL, .value.bval = v.value.sval.size > 0, 0};
  } else if (v.tag == CHAR) {
    return (Value) {BOOL, .value.bval = v.value.cval != '\0', 0};
  } else {
    printf("Error: Cannot convert '%s' to bool.\n", typeOf(v));
    exit(1);
  }
}

Value toFloat(Value v) {
  if (v.tag == FLOAT) {
    return v;
  } else if (v.tag == INT) {
    return (Value) {FLOAT, .value.fval = (float)v.value.i64val, 0};
  } else if (v.tag == STRING) {
    float num = atof(v.value.sval.str);
    if (num == 0) {
      printf("Error: Cannot convert '%s' to float.\n", v.value.sval.str);
      exit(1);
    }
    return (Value) {FLOAT, .value.fval = num, 0};
  } else if (v.tag == CHAR) {
    if (v.value.cval >= '0' && v.value.cval <= '9') {
      return (Value) {FLOAT, .value.fval = v.value.cval - '0', 0};
    } else {
      printf("Error: Cannot convert '%c' to float.\n", v.value.cval);
      exit(1);
    }
  } else {
    printf("Error: Cannot convert '%s' to float.\n", typeOf(v));
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

Value toChar(Value v) {
  if (v.tag == CHAR) {
    return v;
  } else if (v.tag == INT) {
    return (Value) {CHAR, .value.cval = v.value.i64val, 0};
  } else if (v.tag == FLOAT) {
    return (Value) {CHAR, .value.cval = (char)v.value.fval, 0};
  } else if (v.tag == STRING) {
    if (strlen(v.value.sval.str) == 1) {
      return (Value) {CHAR, .value.cval = v.value.sval.str[0], 0};
    } else {
      printf("Error: Cannot convert '%s' to char.\n", v.value.sval.str);
      exit(1);
    }
  } else {
    printf("Error: Cannot convert '%s' to char.\n", typeOf(v));
    exit(1);
  }
}

Value toString(Value v) {
  if (v.tag == STRING) {
    return v;
  } else if (v.tag == INT) {
    char *str = (char *) malloc(20);
    sprintf(str, "%ld", v.value.i64val);
    return (Value) {STRING, .value.sval = {str, strlen(str)}, 0};
  } else if (v.tag == FLOAT) {
    char *str = (char *) malloc(20);
    sprintf(str, "%f", v.value.fval);
    return (Value) {STRING, .value.sval = {str, strlen(str)}, 0};
  } else if (v.tag == CHAR) {
    char *str = (char *) malloc(2);
    str[0] = v.value.cval;
    str[1] = '\0';
    return (Value) {STRING, .value.sval = {str, strlen(str)}, 0};
  } else {
    printf("Error: Cannot convert '%s' to string.\n", typeOf(v));
    exit(1);
  }
}

/* STD type conversions */
bool toStdBool(Value v) { return toBool(v).value.bval; }
char *toStdString(Value v) { return toString(v).value.sval.str; }
int toStdInt(Value v) { return toInt(v).value.i64val; }
float toStdFloat(Value v) { return toFloat(v).value.fval; }
char toStdChar(Value v) { return toChar(v).value.cval; }

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
  } else if (data.tag == BOOL) {
    printf("%s", data.value.bval ? "true" : "false");
  }
}

void println(Value data) {
  print(data);
  printf("\n");
}
// End mysdtlib
