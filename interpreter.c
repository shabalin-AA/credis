#include "interpreter.h"

#include "hashtable.h"
#include <ctype.h>
#include <stdio.h>
#include <assert.h>

#define WORD_LEN 64

Hashtable db = {0};


bool is_number(char* str) 
{
  bool res = true;
  size_t i = 0;
  if (str[i] == '-') i++;
  res = res && isdigit(str[i++]);
  for (; i < strlen(str); i++) {
    res = res && (isdigit(str[i]) || (str[i] == '.'));
  }
  return res;
}

Token token_new(Str str)
{
  Token res = {0};
  if (!strcmp(str.content, "SET")) {
    res.type = T_SET;
  }
  else if (!strcmp(str.content, "GET")) {
    res.type = T_GET;
  }
  else if (!strcmp(str.content, "DEL")) {
    res.type = T_DEL;
  }
  else if (!strcmp(str.content, "EXISTS")) {
    res.type = T_EXISTS;
  }
  else if (!strcmp(str.content, "KEYS")) {
    res.type = T_KEYS;
  }
  else if (!strcmp(str.content, "RENAME")) {
    res.type = T_RENAME;
  }
  else if (!strcmp(str.content, "TYPE")) {
    res.type = T_TYPE;
  }
  else if (is_number(str.content)) {
    res.type = T_NUM;
    res.value = value_number(atof(str.content));
  }
  else if (str.content[0] == '"' && str.content[str.len-1] == '"') {
    res.type = T_STR;
    Str without_quotes = str_new(&str.content[1]);
    without_quotes.content[without_quotes.len-1] = 0;
    res.value = value_str(without_quotes);
  }
  else {
    res.type = T_STR;
    res.value = value_str(str_new(str.content));
  }
  //printf("type: %d; str: %s\n", res.type, str.content);
  return res;
}

bool command_set(TokenList tokens) 
{
  if (tokens.items[0].type == T_SET) {
    assert(tokens.length > 1 && "USAGE: SET <key> <value>");
    int i = 1;
    Token key_tok = tokens.items[i];
    assert(key_tok.type == T_STR && "Expected key");
    Str key = str_value(key_tok.value);
    assert((size_t)i+1 < tokens.length && "Expected value");
    Token val_tok = tokens.items[i+1];
    Value val = val_tok.value;
    ht_insert(&db, key, val);
    printf("OK\n");
    return true;
  }
  return false;
}

bool command_get(TokenList tokens)
{
  if (tokens.items[0].type == T_GET) {
    assert(tokens.length > 1 && "USAGE: SET <key> <value>");
    int i = 1;
    Token key_tok = tokens.items[i];
    assert(key_tok.type == T_STR && "Expected key");
    Str key = str_value(key_tok.value);
    List values = ht_get(&db, key);
    list_print(&values);
    return true;
  }
  return false;
}

void interpret(TokenList tokens)
{
  bool did = 
    command_set(tokens) ||
    command_get(tokens);
  assert(did && "Command didn't recognized");
}

TokenList tokenize(Str line)
{
  TokenList tokens = {0};
  DA_INIT(&tokens, Token);
  size_t len = 0;
  char word[WORD_LEN] = {0};
  char* wb = line.content;
  char sep = ' ';
  for (size_t i = 0; i < line.len; i++, len++) {
    if (line.content[i] == sep) {
      if (len > 0) {
        strncpy(word, wb, len);
        word[len] = 0;
        DA_PUSH(&tokens, token_new((Str){.content = word, .len = len}));
      }
      len = 0;
      i++;
      while (line.content[i] == sep) i++;
      wb = &line.content[i];
    }
  }
  if (len > 0) {
    strncpy(word, wb, len);
    word[len] = 0;
    DA_PUSH(&tokens, token_new((Str){.content = word, .len = len}));
  }
  return tokens;
}
