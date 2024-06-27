#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_


#include "str.h"
#include "list.h"
#include "value.h"

typedef struct {
  Str key;
  List chain;
} Pair;

typedef struct {
  size_t capacity;
  size_t length;
  Pair* items;
} Hashtable;

void ht_insert(Hashtable* table, Str key, Value value);
void ht_remove(Hashtable* table, Str key);
List ht_get(Hashtable* table, Str key) ;
void ht_free(Hashtable* table);


#endif