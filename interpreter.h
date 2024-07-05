#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_ 


#include "da.h"
#include "value.h"
#include "str.h"

typedef enum {
  // commands
  T_PING,
  T_SET,
  T_GET,
  T_DEL,
  T_EXISTS,
  T_KEYS,
  T_RENAME,
  T_TYPE,
  T_PX,
  // values
  T_STR,
  T_NUM,
} TokenType;

typedef struct {
  Value value;
  TokenType type;
} Token;

DA_DEF(Token, TokenList);

TokenList tokenize(Str line);
char* interpret(TokenList tokens);


#endif
