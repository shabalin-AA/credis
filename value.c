#include "value.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "str.h"
#include "list.h"
#include "set.h"
#include "hashtable.h"

Value value_nil()
{
  Value res = (Value) {
    .type = NIL,
    .integer = 0
  };
  return res;
}

Value value_int(int val) 
{
  Value res = (Value) {
    .type = INTEGER,
    .integer = val
  };
  return res;
}

Value value_str(Str val) 
{
  Value res = (Value) {
    .type = STR,
    .ptr = val
  };
  return res;
}

Value value_list(List* val)
{
  Value res = (Value) {
    .type = LIST,
    .ptr = val
  };
  return res;
}

int integer_value(Value value)
{
  assert((value.type == INTEGER) && "Wrong type. Expected INTEGER");
  return value.integer;
}

Str str_value(Value value)
{
  assert((value.type == STR) && "Wrong type. Expected STR");
  return (Str)value.ptr;
}

List* list_value(Value value)
{
  assert((value.type == LIST) && "Wrong type. Expected LIST");
  return (List*)value.ptr;
}

void free_value(Value value) 
{
  switch (value.type) {
    case STR:
      free(value.ptr);
      break;
    case NIL:
      break;
    case INTEGER:
      break;
    case LIST:
      free_list(value.ptr);
      break;
    case HASH:
      ht_free(value.ptr);
      break;
    case SET:
      set_free(value.ptr);
      break;
  }
}

bool value_eq(Value v1, Value v2)
{
  if (v1.type == v2.type) {
    if (v1.type == INTEGER) return v1.integer == v2.integer;
    return v1.ptr == v2.ptr;
  } else return false;
}