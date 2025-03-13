#ifndef CLASSES_H
#define CLASSES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mystdlib.h"

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

#endif