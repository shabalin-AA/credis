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
    .number = 0
  };
  return res;
}

Value value_number(double val) 
{
  Value res = (Value) {
    .type = NUMBER,
    .number = val
  };
  return res;
}

Value value_str(Str val) 
{
  Value res = (Value) {
    .type = STR,
    .str = val
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

double number_value(Value value)
{
  assert((value.type == NUMBER) && "Wrong type. Expected NUMBER");
  return value.number;
}

Str str_value(Value value)
{
  assert((value.type == STR) && "Wrong type. Expected STR");
  return value.str;
}

List* list_value(Value value)
{
  assert((value.type == LIST) && "Wrong type. Expected LIST");
  return (List*)value.ptr;
}

void value_free(Value value) 
{
  switch (value.type) {
    case STR:
      str_free(&value.str);
      break;
    case NIL:
      break;
    case NUMBER:
      break;
    case LIST:
      list_free(value.ptr);
      break;
    case HASH:
      ht_free(value.ptr);
      break;
    case SET:
      set_free(value.ptr);
      break;
  }
}

int value_print(Value value, char* dest)
{
  switch (value.type) {
    case STR:
      return str_print(&value.str, dest);
    case NIL:
      return sprintf(dest, "(nil)\n");
    case NUMBER:
      return sprintf(dest, "%lf\n", value.number);
    case LIST:
      return list_print(value.ptr, dest);
    case HASH:
      return ht_print(value.ptr, dest);
    case SET:
      return set_print(value.ptr, dest);
  }
}

bool value_eq(Value v1, Value v2)
{
  if (v1.type == v2.type) {
    if (v1.type == NUMBER) return v1.number == v2.number;
    else if (v1.type == STR) {
      return str_eq(v1.str, v2.str);
    }
    return v1.ptr == v2.ptr;
  } 
  else return false;
}