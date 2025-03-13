#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mystdlib.h"

// * INIZIO CLASSE 'List'

typedef struct List List;

// Struttura della classe
struct List {
  Object base;  // Oggetto con metodi generici da implementare

  Value *items;
  int size;

  // Puntatori ai metodi
  Value (*append)     (List *this, Value d);
  Value (*pop)        (List *this, Value index);
  Value (*len)        (List *this);
  Value (*__get__)    (List *this, Value index);
};

// Metodi

Value List_len(List *this) {
  return (Value) {INT, .value.ival = this->size};
}

Value List_append(List *this, Value d) {
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

Value List__to_string__(void *self) {
  List *this = (List *) self;  // Cast da Object a List

  Value result = {STRING, .value.sval = {"[", 1}};

  for (int i = 0; i < this->size; i++) {
    result = concat_strings(result, String(this->items[i]));
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

  // Metodi specifici della classe
  obj->append = List_append;
  obj->pop = List_pop;
  obj->len = List_len;
  obj->__get__ = List__get__;

  return obj;
}

// * FINE CLASSE 'List'

Value do_something(Value list, Value arg) {

  // Metodo ottimale per gestire una chiamata ad un metodo
  if (list.tag == OBJECT && list.value.obj_ptr && ((List*)list.value.obj_ptr)->append) {
    ((List*)list.value.obj_ptr)->append(((List*)list.value.obj_ptr), arg);
  } else {
    printf("Error: Cannot call 'append' on '%s'\n", typeOf(list));
    exit(1);
  }
  return NONE;
}

int main() {
  Value list = {OBJECT, .value.obj_ptr = (Object*)List__new__(2, 3)};
  printf("list = new List()\n");

  printf("print(list) -> ");
  println(list);
  printf("\n");

  printf("list.append(\"ciao\")\n");
  ((List*)list.value.obj_ptr)->append((List*)list.value.obj_ptr, (Value){STRING, .value.sval = {"ciao", 4}});

  printf("print(list) -> ");
  println(list);
  printf("\n");

  printf("list.append(2)\n");
  ((List*)list.value.obj_ptr)->append((List*)list.value.obj_ptr, (Value){INT, .value.ival = 2});

  printf("print(list) -> ");
  println(list);
  printf("\n");

  printf("list.append(3.14)\n");
  ((List*)list.value.obj_ptr)->append((List*)list.value.obj_ptr, (Value){FLOAT, .value.fval = 3.14});

  printf("list.pop(1)\n");
  ((List*)list.value.obj_ptr)->pop((List*)list.value.obj_ptr, (Value){INT, .value.ival = 1});

  printf("print(list) -> ");
  println(list);

  printf("list.__to_string__() -> ");
  println(((List*)list.value.obj_ptr)->base.__to_string__((List*)list.value.obj_ptr));
  return 0;
}