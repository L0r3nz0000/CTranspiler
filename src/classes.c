#include "classes.h"

// * INIZIO CLASSE generica

typedef Object Class1;

// Metodi di classe
Value Class1_append(Object *self, Value d) {
  Class1 *this = (Class1 *) self;

  // Implementazione del metodo

  return NONE;
}

Value Class1__to_string_(Object *self) {
  Class1 *this = (Class1 *) self;

  return toString("Class1");
}

// Distruttore
Value Class1__free_(Object *self) {
  Class1 *this = (Class1 *) self;

  // *Deallocare tutti i puntatori della classe

  free(this);
}

// Costruttore
Value Class1__new_() {
  Class1 *obj = malloc(sizeof(Class1));
  if (obj == NULL) {
    perror("Errore malloc");
    exit(1);
  }

  // Override dei metodi generici
  obj->_free_ = Class1__free_;
  obj->_to_string_ = Class1__to_string_;
  obj->class_name = "Class1";

  // Registra i metodi specifici della classe
  register_methods(obj, (MethodEntry[]) {
    {"append", Class1_append},
  }, 1);

  register_fields(obj, (FieldEntry[]) {
    {"field1", NONE},
    {"field2", toInt(3)},
  }, 2);

  // Costructor logic
  // ...

  return (Value) {OBJECT, .value.obj_ptr = obj};
}

// * FINE CLASSE 

int main() {
  Value obj = Class1__new_();
  printf("nome classe: ");
  println(obj.value.obj_ptr->_to_string_(obj.value.obj_ptr));
  set_field(obj.value.obj_ptr, "field1", toString("valore"));
  set_field(obj.value.obj_ptr, "field2", toInt(42));

  printf("field1: ");
  print(get_field(obj.value.obj_ptr, "field1"));
  printf("\nfield2: ");
  println(get_field(obj.value.obj_ptr, "field2"));
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

/*
class Somma {
  @ a;
  @ b;

  fun _new_(a, b) => {
    this.a = a;
    this.b = b;
  }

  fun somma() => {
    <- this.a + this.b;
  }
}

fun main() => {
  @ object = Somma__new_(3, 4);

  println("a: " + String(object.a));
  println("b: " + String(object.b));
  println("somma: " + String(object.somma()));
  <- 0;
}
*/