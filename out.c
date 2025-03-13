// Start mysdtlib
#ifndef MYSTDLIB_H_
#define MYSTDLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

typedef struct Object Object;

typedef enum {
  NONE_TYPE,
  CHAR,
  INT,
  FLOAT,
  STRING,
  BOOL,
  TYPE,
  OBJECT,
} DataType;

typedef struct {
  char *str;
  int size;
} string;

typedef struct {
  DataType tag;
  union {
    char cval;
    long ival;
    double fval;
    string sval;
    bool bval;
    DataType typeval;

    Object *obj_ptr;
  } value;
} Value;

// Ogni Classe deve ereditare da questa struttura e implementare i metodi
typedef struct Object {
  char *class_name;
  void (*__free__)(void *self);
  Value (*__to_string__)(void *self);
} Object;

void print(Value data);

#define NONE ((Value){NONE_TYPE, {0}})

Value type(Value v) {
  return (Value) {TYPE, .value.typeval = v.tag};
}

Value concat_strings(Value string1, Value string2) {
  if (string1.tag != STRING || string2.tag != STRING) {
    printf("Error: Invalid data types for concatenation. Cannot concatenate '");
    print(type(string1)); printf("' and '"); print(type(string2)); printf("'.\n");
    exit(1);
  }

  int size = string1.value.sval.size + string2.value.sval.size;
  char *str = (char *) malloc(size + 1);
  strncpy(str, string1.value.sval.str, string1.value.sval.size);
  strncpy(str + string1.value.sval.size, string2.value.sval.str, string2.value.sval.size);
  str[size] = '\0';

  return (Value) {STRING, .value.sval = {str, size}};
}

void trim_float(char *buffer, double fval) {
  snprintf(buffer, sizeof(buffer), "%.*g", DBL_DIG, fval);

  int len = strlen(buffer);
  
  // Se c'è un punto decimale toglie gli zeri
  if (strchr(buffer, '.')) {
    while (len > 0 && buffer[len - 1] == '0') {
      buffer[len - 1] = '\0';
      len--;
    }
    // Se rimane solo il punto lo elimina
    if (len > 0 && buffer[len - 1] == '.') {
      buffer[len - 1] = '\0';
    }
  }
}

Value add_operator(Value a, Value b) {  // Operatore +
  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.ival = a.value.ival + b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval + b.value.fval};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.ival = a.value.ival + b.value.fval};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.ival = a.value.fval + b.value.ival};
  } else if (a.tag == STRING && b.tag == STRING) {
    return concat_strings(a, b);
  } else {
    printf("Error: Invalid data types for '+' operator. Cannot add '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value sub_operator(Value a, Value b) {  // Operatore -
  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.ival = a.value.ival - b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval - b.value.fval};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.ival = a.value.ival - b.value.fval};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.ival = a.value.fval - b.value.ival};
  } else {
    printf("Error: Invalid data types for '-' operator. Cannot subtract '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value mul_operator(Value a, Value b) {  // Operatore *
  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.ival = a.value.ival * b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval * b.value.fval};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.ival = a.value.ival * b.value.fval};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.ival = a.value.fval * b.value.ival};
  } else if (a.tag == STRING && b.tag == INT) {
    if (b.value.ival < 0) {
      printf("Error: Cannot multiply string by negative number.\n");
      exit(1);
    }
    Value result = {STRING, .value.sval = {"", 0}};

    for (int i = 0; i < b.value.ival; i++) {
      result = concat_strings(result, a);
    }
    return result;
  } else if (a.tag == INT && b.tag == STRING) {
    if (a.value.ival < 0) {
      printf("Error: Cannot multiply string by negative number.\n");
      exit(1);
    }
    Value result = {STRING, .value.sval = {"", 0}};

    for (int i = 0; i < a.value.ival; i++) {
      result = concat_strings(result, b);
    }
    return result;
  } else {
    printf("Error: Invalid data types for '*' operator. Cannot multiply '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value div_operator(Value a, Value b) {  // Operatore /
  if ((b.tag == INT && b.value.ival == 0) || (b.tag == FLOAT && b.value.fval == 0)) {
    printf("Error: Zero division error (in module operation) Cannot divide '");
    print(a);
    printf("' by zero.\n");
    exit(1);
  }


  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.ival = a.value.ival / b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.fval = a.value.fval / b.value.fval};
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {FLOAT, .value.ival = a.value.ival / b.value.fval};
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {FLOAT, .value.ival = a.value.fval / b.value.ival};
  } else {
    printf("Error: Invalid data types for '/' operator. Cannot divide '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value mod_operator(Value a, Value b) { // Operatore %
  if (b.tag == INT && b.value.ival == 0) {
    printf("Error: Zero division error (in module operation) Cannot divide '");
    print(a);
    printf("' by zero.\n");
    exit(1);
  }

  if (a.tag == INT && b.tag == INT) {
    return (Value) {INT, .value.ival = a.value.ival % b.value.ival};
  } else {
    printf("Error: Invalid data types for '%%' operator. Cannot divide '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.ival == b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval == b.value.fval};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval == b.value.ival};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.ival == b.value.fval};
  } else if (a.tag == STRING && b.tag == STRING) {
    return (Value) {BOOL, .value.bval = strcmp(a.value.sval.str, b.value.sval.str) == 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval == b.value.cval};
  } else {
    printf("Error: Invalid data types for '==' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_not_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.ival != b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval != b.value.fval};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval != b.value.ival};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.ival != b.value.fval};
  } else if (a.tag == STRING && b.tag == STRING) {
    return (Value) {BOOL, .value.bval = strcmp(a.value.sval.str, b.value.sval.str) != 0};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval != b.value.cval};
  } else {
    printf("Error: Invalid data types for '!=' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_less(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.ival < b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval < b.value.fval};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval < b.value.ival};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.ival < b.value.fval};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval < b.value.cval};
  } else {
    printf("Error: Invalid data types for '<' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_greater(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.ival > b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval > b.value.fval};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval > b.value.ival};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.ival > b.value.fval};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval > b.value.cval};
  } else {
    printf("Error: Invalid data types for '>' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_less_or_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.ival <= b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval <= b.value.fval};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval <= b.value.ival};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.ival <= b.value.fval};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval <= b.value.cval};
  } else {
    printf("Error: Invalid data types for '<=' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_greater_or_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.ival >= b.value.ival};
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.fval >= b.value.fval};
  } else if (a.tag == FLOAT && b.tag == INT) {
    return (Value) {BOOL, .value.bval = a.value.fval >= b.value.ival};
  } else if (a.tag == INT && b.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = a.value.ival >= b.value.fval};
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return (Value) {BOOL, .value.bval = a.value.cval >= b.value.cval};
  } else {
    printf("Error: Invalid data types for '>=' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value Bool(Value v) {
  if (v.tag == BOOL) {
    return v;
  } else if (v.tag == INT) {
    return (Value) {BOOL, .value.bval = v.value.ival != 0};
  } else if (v.tag == FLOAT) {
    return (Value) {BOOL, .value.bval = v.value.fval != 0};
  } else if (v.tag == STRING) {
    return (Value) {BOOL, .value.bval = v.value.sval.size > 0};
  } else if (v.tag == CHAR) {
    return (Value) {BOOL, .value.bval = v.value.cval != '\0'};
  } else {
    printf("Error: Cannot convert '");
    print(type(v)); printf(" to bool.\n");
    exit(1);
  }
}

Value Float(Value v) {
  if (v.tag == FLOAT) {
    return v;
  } else if (v.tag == INT) {
    return (Value) {FLOAT, .value.fval = (float)v.value.ival};
  } else if (v.tag == STRING) {
    float num = atof(v.value.sval.str);
    if (num == 0) {
      printf("Error: Cannot convert '%s' to float.\n", v.value.sval.str);
      exit(1);
    }
    return (Value) {FLOAT, .value.fval = num};
  } else if (v.tag == CHAR) {
    if (v.value.cval >= '0' && v.value.cval <= '9') {
      return (Value) {FLOAT, .value.fval = v.value.cval - '0'};
    } else {
      printf("Error: Cannot convert '%c' to float.\n", v.value.cval);
      exit(1);
    }
  } else {
    printf("Error: Cannot convert '");
    print(type(v)); printf(" to float.\n");
    exit(1);
  }
}

Value Int(Value v) {
  if (v.tag == INT) {
    return v;
  } else if (v.tag == FLOAT) {
    return (Value) {INT, .value.ival = (int)v.value.fval};
  } else if (v.tag == STRING) {
    long num = atoi(v.value.sval.str);
    if (num == 0) {
      float fnum = atof(v.value.sval.str);
      if (fnum == 0) {
        printf("Error: Cannot convert '%s' to int.\n", v.value.sval.str);
        exit(1);
      }
      num = (int) fnum;
    }
    return (Value) {INT, .value.ival = num};
  } else if (v.tag == CHAR) {
    if (v.value.cval >= '0' && v.value.cval <= '9') {
      return (Value) {INT, .value.ival = v.value.cval - '0'};
    } else {
      printf("Error: Cannot convert '%c' to int.\n", v.value.cval);
      exit(1);
    }
  } else {
    printf("Error: Cannot convert '");
    print(type(v)); printf(" to int.\n");
    exit(1);
  }
}

Value Char(Value v) {
  if (v.tag == CHAR) {
    return v;
  } else if (v.tag == INT) {
    return (Value) {CHAR, .value.cval = v.value.ival};
  } else if (v.tag == FLOAT) {
    return (Value) {CHAR, .value.cval = (char)v.value.fval};
  } else if (v.tag == STRING) {
    if (strlen(v.value.sval.str) == 1) {
      return (Value) {CHAR, .value.cval = v.value.sval.str[0]};
    } else {
      printf("Error: Cannot convert '%s' to char.\n", v.value.sval.str);
      exit(1);
    }
  } else {
    printf("Error: Cannot convert '");
    print(type(v)); printf(" to char.\n");
    exit(1);
  }
}

Value String(Value v) {
  if (v.tag == STRING) {
    return v;
  } else if (v.tag == INT) {
    char *str = (char *) malloc(20);
    sprintf(str, "%ld", v.value.ival);
    return (Value) {STRING, .value.sval = {str, strlen(str)}};
  } else if (v.tag == FLOAT) {
    char *str = (char *) malloc(20);
    trim_float(str, v.value.fval);
    return (Value) {STRING, .value.sval = {str, strlen(str)}};
  } else if (v.tag == CHAR) {
    char *str = (char *) malloc(2);
    str[0] = v.value.cval;
    str[1] = '\0';
    return (Value) {STRING, .value.sval = {str, strlen(str)}};
  } else if (v.tag == OBJECT && v.value.obj_ptr && v.value.obj_ptr->__to_string__) {
    return v.value.obj_ptr->__to_string__(v.value.obj_ptr);
  } else {
    printf("Error: Cannot convert '");
    print(type(v)); printf(" to string.\n");
    exit(1);
  }
}

/* STD type conversions */
bool c_bool(Value v)              { return Bool(v).value.bval; }
char c_char(Value v)              { return Char(v).value.cval; }
wchar_t c_wchar(Value v); //! Not supported yet
char c_byte(Value v)              { return Char(v).value.cval; }
unsigned char c_ubyte(Value v)    { return (unsigned char) Char(v).value.cval; }
short c_short(Value v)            { return (short) Int(v).value.ival; }
unsigned short c_ushort(Value v)  { return (unsigned short) Int(v).value.ival; }
int c_int(Value v)                { return Int(v).value.ival; }
unsigned int c_uint(Value v) {}
long c_long(Value v)              { return Int(v).value.ival; }
unsigned long c_ulong(Value v)    { return (unsigned long) Int(v).value.ival; }
long long c_longlong(Value v)     { return Int(v).value.ival; }
long long c_ulonglong(Value v)    { return (unsigned long) Int(v).value.ival; }
unsigned long c_size_t(Value v)   { return (unsigned long) Int(v).value.ival; }
long c_time_t(Value v)            { return Int(v).value.ival; }
float c_float(Value v)            { return (float) Float(v).value.fval; }
double c_double(Value v)          { return Float(v).value.fval; }
long double c_longdouble(Value v) { return Float(v).value.fval; }
char *c_char_p(Value v)           { return String(v).value.sval.str; }
wchar_t *c_wchar_p(Value v); //! Not supported yet
void *c_void_p(Value v) {
  switch (v.tag) {
    case CHAR:
      char *c_ptr = (void*)malloc(sizeof(v.value.cval));
      if (c_ptr != NULL) {
        *c_ptr = v.value.cval;
      }
      return (void*) c_ptr;
      break;
    case INT:
      long *l_ptr = (void*)malloc(sizeof(long));
      if (l_ptr != NULL) {
        *l_ptr = v.value.ival;
      }
      return (void*) l_ptr;
      break;
    case FLOAT:
      long *f_ptr = (void*)malloc(sizeof(double));
      if (f_ptr != NULL) {
        *f_ptr = v.value.fval;;
      }
      return (void*) f_ptr;
      break;
    case STRING:
      return (void*) v.value.sval.str;
      break;
    default:
      printf("Error: Cannot convert '");
      print(type(v)); printf("' to (void *)\n");
      exit(1);
      break;
  }
}

Value toString(char *str) {
  return (Value) {STRING, .value.sval = {str, strlen(str)}};
}

Value toChar(char c) {
  return (Value) {CHAR, .value.cval = c};
}

Value toInt(int i) {
  return (Value) {INT, .value.ival = i};
}

Value toFloat(float f) {
  return (Value) {FLOAT, .value.fval = f};
}

Value toBool(bool b) {
  return (Value) {BOOL, .value.bval = b};
}

void print(Value data) {
  if (data.tag == NONE_TYPE) {
    printf("None\n");
    return;
  } else if (data.tag == INT) {
    printf("%d", data.value.ival);
  } else if (data.tag == FLOAT) {
    char str[20];
    trim_float(str, data.value.fval);
    printf("%s", str);
  } else if (data.tag == CHAR) {
    printf("%c", data.value.cval);
  } else if (data.tag == STRING) {
    printf("%s", data.value.sval.str);
  } else if (data.tag == BOOL) {
    printf("%s", data.value.bval ? "true" : "false");
  } else if (data.tag == OBJECT && data.value.obj_ptr && data.value.obj_ptr->__to_string__) {
    print(data.value.obj_ptr->__to_string__(data.value.obj_ptr));
  } else if (data.tag == TYPE) {
    switch (data.value.typeval) {
      case CHAR: printf("char"); break;
      case INT: printf("int"); break;
      case FLOAT: printf("float"); break;
      case STRING: printf("string"); break;
      case BOOL: printf("bool"); break;
      case OBJECT: 
        if (data.value.obj_ptr->class_name) {
          printf("<Object '%s'>", data.value.obj_ptr->class_name);
        } else {
          printf("<Unnamed Object>");
        }
        break;
      default: printf("<Unknown Object '%d'>", data.value.typeval); break;
    }
  } else {
    printf("<Unknown Object>");
  }
}

void println(Value data) {
  print(data);
  printf("\n");
}

Value readln() {
  char c = '\0';
  char *buffer = malloc(sizeof(char));

  int i = 0;
  while ((c = getchar()) != '\n') {
    buffer[i] = c;
    i++;
    buffer = realloc(buffer, (i + 1) * sizeof(char));
  }
  buffer[i] = '\0';
  return (Value) {STRING, .value.sval = {buffer, i}};
}

// End mysdtlib
#endif
#include <unistd.h>
int main() {
println(((Value) {STRING, .value.sval = {"ciao", 4}}));
print(((Value) {STRING, .value.sval = {"Inserisci un numero: ", 21}}));
Value num = Int(readln());
Value i = ((Value) {INT, .value.ival = 0});
while (c_bool(is_less(i, add_operator(mul_operator(num, ((Value) {INT, .value.ival = 2})), ((Value) {INT, .value.ival = 1}))))) {
Value spaces = sub_operator(num, Int(div_operator(i, ((Value) {INT, .value.ival = 2}))));
print(mul_operator(((Value) {STRING, .value.sval = {" ", 1}}), spaces));
println(mul_operator(((Value) {STRING, .value.sval = {"*", 1}}), i));
i = add_operator(i, ((Value) {INT, .value.ival = 2}));
}
return c_int(((Value) {INT, .value.ival = 0}));
}
