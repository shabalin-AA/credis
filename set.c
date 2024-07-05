#include "set.h"

#include <stdbool.h>
#include <stdio.h>

#define LEN 8
#define FACTOR 2

bool set_insert(Set* set, Value value)
{
  if (set->items == NULL) {
    set->capacity = LEN;
    set->items = malloc(sizeof(Value) * set->capacity);
    set->length = 0;
  }
  for (size_t i = 0; i < set->length; i++) {
    if (value_eq(set->items[i], value)) return false;
  }
  if (set->length == set->capacity) {
    set->capacity *= FACTOR;
    set->items = realloc(set->items, sizeof(set->items[0]) * set->capacity);
  }
  set->items[set->length++] = value;
  return true;
}

void set_remove(Set* set, ssize_t index)
{
  if (set->length < 1 || index < 0 || (size_t)index >= set->length) return;
  memmove(
    &set->items[index], 
    &set->items[index+1], 
    (set->length-index-1)*sizeof(set->items[0])
  );
  set->length--;
}

ssize_t set_indexof(Set* set, Value value)
{
  for (size_t i = 0; i < set->length; i++) {
    if (value_eq(set->items[i], value)) return i;
  }
  return -1;
}

void set_free(Set* set)
{
  for (size_t i = 0; i < set->length; i++) {
    value_free(set->items[i]);
  }
  free(set->items);
  set->items = NULL;
  set->length = 0;
  set->capacity = 0;
}

int set_print(Set* set, char* dest)
{
  int offset = 0;
  offset += sprintf(dest + offset, "<");
  for (size_t i = 0; i < set->length; i++) {
    offset += sprintf(dest + offset, "\n  ");
    offset += value_print(set->items[i], dest + offset) - 1;
  }
  offset += sprintf(dest + offset, ">\n");
  return offset;
}