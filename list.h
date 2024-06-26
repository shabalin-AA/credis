#ifndef _LIST_H_
#define _LIST_H_


#include "value.h"

struct ListNode {
  Value value;
  struct ListNode* next;
};

typedef struct ListNode ListNode;

typedef struct {
  ListNode* head;
} List;

void lpush(List* list, Value value);
void rpush(List* list, Value value);
ListNode* left(List* list);
ListNode* right(List* list);
void lpop(List* list);
void rpop(List* list);
void free_list(List* list);
Value value_list(List* val);
List* list_value(Value value);


#endif