#ifndef _SET_H
#define _SET_H


#include "value.h"
#include <stdbool.h>

typedef struct {
  Value* items;
  size_t capacity;
  size_t length;
} Set;

bool set_insert(Set* set, Value value);
void set_remove(Set* set, ssize_t index);
ssize_t set_indexof(Set* set, Value value);
void set_free(Set* set);


#endif