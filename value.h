#ifndef _VALUE_H_
#define _VALUE_H_


#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "str.h"

typedef enum {
  NIL,
  NUMBER,
  STR,
  LIST,
  SET,
  HASH
} ValueType;

struct Value {
  ValueType type;
  union {
    double number;
    Str str;
    void* ptr;
  };
};

typedef struct Value Value;

Value value_nil();
Value value_number(double val);
Value value_str(Str val);
double number_value(Value value);
Str str_value(Value value);
void value_free(Value value);
/* 
  functions for lists, sets, etc. 
  defined in corresponding files (list.h, set.h, ...) 
  but implemented in value.c
*/
bool value_eq(Value v1, Value v2);
void value_print(Value value);


#endif