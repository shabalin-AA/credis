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
Value lpop(List* list);
Value rpop(List* list);
void list_reverse(List* list);
void free_node(ListNode* node);
void list_free(List* list);
Value value_list(List* val);
List* list_value(Value value);
void list_print(List* list);


#endif