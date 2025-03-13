#include "classes.h"

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

// * INIZIO CLASSE generica

// Struttura della classe
typedef struct {
  Object base;  // Oggetto con metodi generici da implementare

  // Campi
  Value *items;
  int size;
} Class1;

// Metodi di classe
Value Class1_method1(Object *self, Value d) {
  Class1 *this = (Class1 *) self;

  // Implementazione del metodo

  return NONE;
}

Value Class1__to_string__(Object *self) {
  Class1 *this = (Class1 *) self;

  return toString("Class1");
}

// Distruttore
void Class1__free__(Object *self) {
  Class1 *this = (Class1 *) self;

  // *Deallocare tutti i puntatori della classe

  free(this);
}

// Costruttore
Class1 *Class1__new__() {
  Class1 *obj = malloc(sizeof(Class1));
  if (obj == NULL) {
    perror("Errore malloc");
    exit(1);
  }

  // Override dei metodi generici
  obj->base.__free__ = Class1__free__;
  obj->base.__to_string__ = Class1__to_string__;
  obj->base.class_name = "Class1";

  // Registra i metodi specifici della classe
  MethodEntry List_methods[] = {
    {"append", Class1_method1},
  };
  register_methods(obj, List_methods, 1);

  return obj;
}

// * FINE CLASSE 

int main() {
  Value obj = {OBJECT, .value.obj_ptr = (Object*)Class1__new__()};
  println(obj);

  return 0;
}

/*
        AST_CLASS
       /         \
 char** fields  AST_FUNCT* methods


*/