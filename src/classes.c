#include "classes.h"



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
                   AST_CLASS   --> char *name
                 /     |     \
                /      |      \
   char** fields int n_methods AST_METHOD* methods
                               /    |      \
                  Access access char* name  AST_BODY* body
*/