#ifndef _VALUE_H_
#define _VALUE_H_


#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "str.h"

typedef enum {
  NIL,
  INTEGER,
  STR,
  LIST,
  SET,
  HASH
} ValueType;

struct Value {
  ValueType type;
  union {
    int integer;
    void* ptr;
  };
};

typedef struct Value Value;

Value value_nil();
Value value_int(int val);
Value value_str(Str val);
int integer_value(Value value);
Str str_value(Value value);
void free_value(Value value);
/* 
  functions for lists, sets, etc. 
  defined in corresponding files (list.h, set.h, ...) 
  but implemented in value.c
*/
bool value_eq(Value v1, Value v2);


#endif