#ifndef _DA_H_
#define _DA_H_


#include <stdlib.h>

#define DA_0_SIZE 8
#define DA_SCALE 2

#define DA_DEF(T, DA_T) typedef struct { size_t capacity; size_t length; T* items; } DA_T

#define DA_INIT(DA, TYPE) \
  do { \
    (DA)->capacity = DA_0_SIZE; \
    (DA)->length = 0; \
    (DA)->items = malloc(sizeof(TYPE) * (DA)->capacity); \
  } while(0)

#define DA_PUSH(DA, VALUE) \
  do { \
		if ((DA)->items == NULL) { \
	    (DA)->capacity = DA_0_SIZE; \
	    (DA)->length = 0; \
	    (DA)->items = malloc(sizeof(VALUE) * (DA)->capacity); \
		} \
    if ((DA)->length == (DA)->capacity) { \
      (DA)->capacity *= DA_SCALE; \
      (DA)->items = realloc((DA)->items, sizeof(VALUE) * (DA)->capacity); \
    } \
    (DA)->items[(DA)->length++] = VALUE; \
  } while(0)

#define DA_POP(DA) \
  do { \
    (DA)->length--; \
    if ((DA)->length > DA_0_SIZE && (DA)->capacity / (DA)->length >= DA_SCALE) { \
      (DA)->capacity /= DA_SCALE; \
      (DA)->items = realloc((DA)->items, sizeof((DA)->items[0]) * (DA)->capacity); \
    } \
  } while(0)

#define DA_DEINIT(DA) free((DA)->items)
			
		
#endif