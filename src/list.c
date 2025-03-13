#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mystdlib.h"

#define MAX_OBJECTS 100

// Ogni Classe deve ereditare da questa struttura e implementare i metodi
typedef struct Object {
  char *class_name;
  void (*__free__)(void *self);
  Value (*__to_string__)(void *self);
} Object;

typedef struct {
  char *method_name;
  Value (*method)(Object *, Value);
} MethodEntry;

typedef struct {
  void *obj;
  MethodEntry *methods;
  int method_count;
} VMTEntry;

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



// * INIZIO CLASSE 'List'
typedef struct List List;

// Struttura della classe
struct List {
  Object base;  // Oggetto con metodi generici da implementare

  Value *items;
  int size;
};

// Metodi
Value List_append(List *self, Value d) {
  List *this = (List *) self;  // Cast da Object a List

  if (this->size == 0) {
    this->items = malloc(sizeof(Value));
  } else {
    this->items = realloc(this->items, sizeof(Value) * (this->size + 1));
  }

  if (this->items == NULL) {
    printf("Errore nell'allocazione di memoria\n");
    exit(1);
  }

  this->items[this->size] = d;
  this->size ++;

  return NONE;
}

Value List_len(List *this) {
  return (Value) {INT, .value.ival = this->size};
}

Value List__to_string__(void *self) {
  List *this = (List *) self;  // Cast da Object a List

  Value result = {STRING, .value.sval = {"[", 1}};

  for (int i = 0; i < this->size; i++) {
    if (this->items[i].tag == STRING) {
      result = concat_strings(result, (Value){STRING, .value.sval = {"\"", 1}});
      result = concat_strings(result, String(this->items[i]));
      result = concat_strings(result, (Value){STRING, .value.sval = {"\"", 1}});
    } else {
      result = concat_strings(result, String(this->items[i]));
    }
    
    if (i < this->size - 1) {
      result = concat_strings(result, (Value){STRING, .value.sval = {", ", 2}});
    }
  }

  result = concat_strings(result, (Value){STRING, .value.sval = {"]", 1}});

  return result;
}

Value List__get__(List *this, Value index) {
  int int_index = c_int(Int(index));

  if (int_index >= 0 && int_index < this->size) {
    return this->items[int_index];
  } else {
    printf("Error: Attempted to access index '%d', but the list size is '%d'\n", int_index, this->size);
    exit(1);
    return NONE;
  }
}

Value List_pop(List *this, Value index) {
  int int_index = c_int(Int(index));

  if (int_index >= this->size) {
    fprintf(stderr, "Error: Attempted to pop index '%d', but the list size is '%d'\n", index, this->size);
    exit(1);
  }
  
  // Salva il valore da rimuovere
  Value popped = this->items[int_index];

  // Sposta tutti gli elementi successivi indietro di uno
  for (size_t i = int_index; i < this->size - 1; i++) {
    this->items[i] = this->items[i + 1];
  }

  this->size--;

  return popped;
}

// Distruttore
void List__free__(void *self) {
  List *this = (List *) self;  // Cast da Object a List
  free(this->items);
  free(this);
}

// Costruttore
List *List__new__() {
  List *obj = malloc(sizeof(List));
  if (obj == NULL) {
    perror("Errore malloc");
    exit(1);
  }

  // Override dei metodi generici
  obj->base.__free__ = List__free__;
  obj->base.__to_string__ = List__to_string__;
  obj->base.class_name = "List";

  // Registra i metodi specifici della classe
  MethodEntry List_methods[] = {
    {"append", List_append},
    {"pop", List_pop},
    {"len", List_len},
    {"__get__", List__get__},
  };
  register_methods(obj, List_methods, 1);

  return obj;
}

// * FINE CLASSE 'List'

Value do_something(Value list, Value arg) {
  
  return NONE;
}

int main() {
  Value list = {OBJECT, .value.obj_ptr = (Object*)List__new__()};
  do_something(list, (Value) {INT, .value.ival = 1});
  do_something(list, (Value) {INT, .value.ival = 2});
  do_something(list, (Value) {INT, .value.ival = 3});

  println(list);


  return 0;
}

// int main() {
//   // Chiama il costruttore
//   Value list = {OBJECT, .value.obj_ptr = (Object*)List__new__(2, 3)};
//   printf("list = new List()\n");

//   printf("print(list) -> ");
//   println(list);
//   printf("\n");

//   printf("list.append(\"ciao\")\n");
//   ((List*)list.value.obj_ptr)->append((List*)list.value.obj_ptr, (Value){STRING, .value.sval = {"ciao", 4}});

//   printf("print(list) -> ");
//   println(list);
//   printf("\n");

//   printf("list.append(2)\n");
//   ((List*)list.value.obj_ptr)->append((List*)list.value.obj_ptr, (Value){INT, .value.ival = 2});

//   printf("print(list) -> ");
//   println(list);
//   printf("\n");

//   printf("list.append(3.14)\n");
//   ((List*)list.value.obj_ptr)->append((List*)list.value.obj_ptr, (Value){FLOAT, .value.fval = 3.14});

//   printf("list.pop(1)\n");
//   ((List*)list.value.obj_ptr)->pop((List*)list.value.obj_ptr, (Value){INT, .value.ival = 1});

//   printf("print(list) -> ");
//   println(list);
//   return 0;
// }