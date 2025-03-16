#ifndef CLASSES_H
#define CLASSES_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mystdlib.h"

// Ogni Classe deve ereditare da questa struttura e implementare i metodi
// typedef struct Object Object;
// struct Object {
//   char *class_name;
//   Value (*_free_)(Object *self);
//   Value (*_to_string_)(Object *self);
// };

// typedef struct {
//   char *method_name;
//   Value (*method)(Object *, Value);
// } MethodEntry;

// typedef struct {
//   Object *obj;
//   MethodEntry *methods;
//   int method_count;
// } VMTEntry;

typedef struct {
  char *field_name;
  Value value;
} FieldEntry;

typedef struct {
  Object *obj;            // Puntatore all'oggetto originale
  FieldEntry *fields;     // Tabella dei campi
  int field_count;        // Numero di campi registrati
} FieldTableEntry;

#define MAX_OBJECTS 100

FieldTableEntry field_table[MAX_OBJECTS];
int field_count = 0;

// * gestione dei campi

void register_fields(Object *obj, FieldEntry *fields, int count) {
  if (field_count < MAX_OBJECTS) {
    if (!obj || !fields || count <= 0) {
      fprintf(stderr, "Error: Cannot register fields for class '%s', invalid data\n", obj->class_name);
      exit(1);
    }

    FieldEntry *copy = malloc(sizeof(FieldEntry) * count);
    if (!copy) {
      perror("Error allocating field table");
      exit(1);
    }

    for (int i = 0; i < count; i++) {
      copy[i].value = fields[i].value;
      copy[i].field_name = strdup(fields[i].field_name);  // Copia sicura della stringa
      if (!copy[i].field_name) {
        perror("Error allocating method name");
        exit(1);
      }
    }

    field_table[field_count].obj = obj;
    field_table[field_count].fields = copy;
    field_table[field_count].field_count = count;
    field_count++;
  }
}

Value get_field(Object *obj, const char *field_name) {
  for (int i = 0; i < field_count; i++) {
    if (field_table[i].obj == obj) {
      for (int j = 0; j < field_table[i].field_count; j++) {
        if (strcmp(field_table[i].fields[j].field_name, field_name) == 0) {
          return field_table[i].fields[j].value;
        }
      }
      printf("Error: Field %s not found, cannot get\n", field_name);
      exit(1);
    }
  }
  printf("Error: Object not found, cannot get field\n");
  exit(1);
}

void set_field(Object *obj, const char *field_name, Value new_value) {
  for (int i = 0; i < field_count; i++) {
    if (field_table[i].obj == obj) {
      for (int j = 0; j < field_table[i].field_count; j++) {
        if (strcmp(field_table[i].fields[j].field_name, field_name) == 0) {
          field_table[i].fields[j].value = new_value;
          return;
        }
      }
      printf("Error: Field %s not found, cannot set\n", field_name);
      exit(1);
    }
  }
  printf("Error: Object not found, cannot set field\n");
  exit(1);
}

// * fine gestione campi

// void register_methods(Object *obj, MethodEntry *methods, int method_count) {
//   if (vmt_count < MAX_OBJECTS) {
//     if (!obj || !methods || method_count <= 0) {
//       fprintf(stderr, "Error: Cannot register methods, invalid data\n");
//       exit(1);
//     }

//     // Allocazione sicura della tabella dei metodi
//     MethodEntry *copy = malloc(sizeof(MethodEntry) * method_count);
//     if (!copy) {
//       perror("Error allocating method table");
//       exit(1);
//     }

//     for (int i = 0; i < method_count; i++) {
//       copy[i].method = methods[i].method;
//       copy[i].method_name = strdup(methods[i].method_name);  // Copia sicura della stringa
//       if (!copy[i].method_name) {
//         perror("Error allocating method name");
//         exit(1);
//       }
//     }

//     vmt_table[vmt_count].obj = obj;
//     vmt_table[vmt_count].methods = copy;
//     vmt_table[vmt_count].method_count = method_count;
//     vmt_count++;
//   }
// }

// Value call_method(Object *obj, const char *method_name, Value arg) {
//   for (int i = 0; i < vmt_count; i++) {
//     if (vmt_table[i].obj == obj) {
//       for (int j = 0; j < vmt_table[i].method_count; j++) {
//         if (strcmp(vmt_table[i].methods[j].method_name, method_name) == 0) {
//           return vmt_table[i].methods[j].method(obj, arg);
//         }
//       }
//       printf("Error: Method %s() not implemented for this object\n", method_name);
//       return NONE;
//     }
//   }
//   printf("Error: Object not found in VMT\n");
//   return NONE;
// }

#endif