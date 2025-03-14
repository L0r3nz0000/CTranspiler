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

#define NONE ((Value){NONE_TYPE, {0}})

// * INIZIO GESTIONE CLASSI
struct Object {
  char *class_name;
  Value (*__free__)(Object *self);
  Value (*__to_string__)(Object *self);
};

typedef struct {
  char *method_name;
  Value (*method)(Object *, Value);
} MethodEntry;

typedef struct {
  void *obj;
  MethodEntry *methods;
  int method_count;
} VMTEntry;

#define MAX_OBJECTS 100

VMTEntry vmt_table[MAX_OBJECTS];
int vmt_count = 0;

void register_methods(void *obj, MethodEntry *methods, int method_count) {
  if (vmt_count < MAX_OBJECTS) {
    if (!obj || !methods || method_count <= 0) {
      fprintf(stderr, "Error: Cannot register methods, invalid data\n");
      return;
    }

    // Allocazione sicura della tabella dei metodi
    MethodEntry *copy = malloc(sizeof(MethodEntry) * method_count);
    if (!copy) {
      perror("Error allocating method table");
      exit(1);
    }

    for (int i = 0; i < method_count; i++) {
      copy[i].method = methods[i].method;
      copy[i].method_name = strdup(methods[i].method_name);  // Copia sicura della stringa
      if (!copy[i].method_name) {
        perror("Error allocating method name");
        exit(1);
      }
    }

    vmt_table[vmt_count].obj = obj;
    vmt_table[vmt_count].methods = copy;
    vmt_table[vmt_count].method_count = method_count;
    vmt_count++;
  }
}


Value call_method(void *obj, const char *method_name, Value arg) {
  for (int i = 0; i < vmt_count; i++) {
    if (vmt_table[i].obj == obj) {
      for (int j = 0; j < vmt_table[i].method_count; j++) {
        if (strcmp(vmt_table[i].methods[j].method_name, method_name) == 0) {
          return vmt_table[i].methods[j].method(obj, arg);
        }
      }
      printf("Error: Method %s() not implemented for this object\n", method_name);
      return NONE;
    }
  }
  printf("Error: Object not found in VMT\n");
}

// * FINE GESTIONE CLASSI

void print(Value data);

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
    return toInt(c_int(a) + c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    float result = c_float(a) + c_float(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else  if (a.tag == INT && b.tag == FLOAT) {
    float result = c_int(a) + c_float(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else  if (a.tag == FLOAT && b.tag == INT) {
    float result = c_float(a) + c_int(b);
    return (int)result == result ? toInt(result) : toFloat(result);
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
    return toInt(c_int(a) - c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    float result = c_float(a) - c_float(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else  if (a.tag == INT && b.tag == FLOAT) {
    float result = c_int(a) - c_float(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else  if (a.tag == FLOAT && b.tag == INT) {
    float result = c_float(a) - c_int(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else {
    printf("Error: Invalid data types for '-' operator. Cannot subtract '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value mul_operator(Value a, Value b) {  // Operatore *
  if (a.tag == INT && b.tag == INT) {
    return toInt(c_int(a) * c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    float result = c_float(a) * c_float(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else  if (a.tag == INT && b.tag == FLOAT) {
    return toFloat(c_int(a) * c_float(b));
  } else  if (a.tag == FLOAT && b.tag == INT) {
    return toFloat(c_float(a) * c_int(b));
  } else if (a.tag == STRING && b.tag == INT) {
    if (b.value.ival < 0) {
      printf("Error: Cannot multiply string by negative number.\n");
      exit(1);
    }
    Value result = toString("");

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
  if ((b.tag == INT && c_int(b) == 0) || (b.tag == FLOAT && c_float(b) == 0)) {
    printf("Error: Zero division error (in module operation) Cannot divide '");
    print(a);
    printf("' by zero.\n");
    exit(1);
  }


  if (a.tag == INT && b.tag == INT) {
    float result = c_int(a) / c_int(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    float result = c_float(a) / c_float(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else  if (a.tag == INT && b.tag == FLOAT) {
    float result = c_int(a) / c_float(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else  if (a.tag == FLOAT && b.tag == INT) {
    float result = c_float(a) / c_int(b);
    return (int)result == result ? toInt(result) : toFloat(result);
  } else {
    printf("Error: Invalid data types for '/' operator. Cannot divide '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value mod_operator(Value a, Value b) { // Operatore %
  if (b.tag == INT && c_int(b) == 0) {
    printf("Error: Zero division error in module operation Cannot divide '");
    print(a);
    printf("' by zero.\n");
    exit(1);
  }

  if (a.tag == INT && b.tag == INT) {
    return toInt(c_int(a) % c_int(b));
  } else {
    printf("Error: Invalid data types for '%%' operator. Cannot divide '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return toBool(c_int(a) == c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return toBool(c_float(a) == c_float(b));
  } else if (a.tag == FLOAT && b.tag == INT) {
    return toBool(c_float(a) == c_int(b));
  } else if (a.tag == INT && b.tag == FLOAT) {
    return toBool(c_int(a) == c_float(b));
  } else if (a.tag == STRING && b.tag == STRING) {
    return toBool(strcmp(c_char_p(a), c_char_p(b)) == 0);
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return toBool(c_char(a) == c_char(b));
  } else {
    printf("Error: Invalid data types for '==' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_not_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return toBool(c_int(a) != c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return toBool(c_float(a) != c_float(b));
  } else if (a.tag == FLOAT && b.tag == INT) {
    return toBool(c_float(a) != c_int(b));
  } else if (a.tag == INT && b.tag == FLOAT) {
    return toBool(c_int(a) != c_float(b));
  } else if (a.tag == STRING && b.tag == STRING) {
    return toBool(strcmp(c_char_p(a), c_char_p(b)) != 0);
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return toBool(c_char(a) != c_char(b));
  } else {
    printf("Error: Invalid data types for '!=' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_less(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return toBool(c_int(a) < c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return toBool(c_float(a) < c_float(b));
  } else if (a.tag == FLOAT && b.tag == INT) {
    return toBool(c_float(a) < c_int(b));
  } else if (a.tag == INT && b.tag == FLOAT) {
    return toBool(c_int(a) < c_float(b));
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return toBool(c_char(a) < c_char(b));
  } else {
    printf("Error: Invalid data types for '<' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_greater(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return toBool(c_int(a) > c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return toBool(c_float(a) > c_float(b));
  } else if (a.tag == FLOAT && b.tag == INT) {
    return toBool(c_float(a) > c_int(b));
  } else if (a.tag == INT && b.tag == FLOAT) {
    return toBool(c_int(a) > c_float(b));
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return toBool(c_char(a) > c_char(b));
  } else {
    printf("Error: Invalid data types for '>' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_less_or_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return toBool(c_int(a) <= c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return toBool(c_float(a) <= c_float(b));
  } else if (a.tag == FLOAT && b.tag == INT) {
    return toBool(c_float(a) <= c_int(b));
  } else if (a.tag == INT && b.tag == FLOAT) {
    return toBool(c_int(a) <= c_float(b));
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return toBool(c_char(a) <= c_char(b));
  } else {
    printf("Error: Invalid data types for '<=' operator. Cannot compare '");
    print(type(a)); printf("' and '"); print(type(b)); printf("'.\n");
    exit(1);
  }
}

Value is_greater_or_equal(Value a, Value b) {
  if (a.tag == INT && b.tag == INT) {
    return toBool(c_int(a) >= c_int(b));
  } else if (a.tag == FLOAT && b.tag == FLOAT) {
    return toBool(c_float(a) >= c_float(b));
  } else if (a.tag == FLOAT && b.tag == INT) {
    return toBool(c_float(a) >= c_int(b));
  } else if (a.tag == INT && b.tag == FLOAT) {
    return toBool(c_int(a) >= c_float(b));
  } else if (a.tag == CHAR && b.tag == CHAR) {
    return toBool(c_char(a) >= c_char(b));
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
    return is_not_equal(v, toInt(0));
  } else if (v.tag == FLOAT) {
    return is_not_equal(v, toFloat(0));
  } else if (v.tag == STRING) {
    return is_greater(v, toInt(0));
  } else if (v.tag == CHAR) {
    return is_not_equal(v, toChar('\0'));
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
  return toString(buffer);
}

// End mysdtlib
#endif
