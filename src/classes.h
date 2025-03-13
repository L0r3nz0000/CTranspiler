#ifndef CLASSES_H
#define CLASSES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mystdlib.h"

// Ogni Classe deve ereditare da questa struttura e implementare i metodi
typedef struct Object Object;
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

void register_methods(Object *obj, MethodEntry *methods, int method_count) {
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

Value call_method(Object *obj, const char *method_name, Value arg) {
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

#endif