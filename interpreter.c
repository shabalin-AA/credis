#include "interpreter.h"

#include "hashtable.h"
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

#define WORD_LEN 64

static Hashtable db = {0};
static Hashtable expirations = {0};
static char response[1024];


bool is_number(char* str) 
{
  bool res = true;
  size_t i = 0;
  if (str[i] == '-') i++;
  res = res && isdigit(str[i++]);
  unsigned dots = 0;
  for (; i < strlen(str); i++) {
    if (str[i] == '.') dots++;
    res = res && (isdigit(str[i]) || (str[i] == '.')) && dots <= 1;
  }
  return res;
}

Token token_new(Str str)
{
  Token res = {0};
  if (!strcmp(str.content, "PING")) {
    res.type = T_PING;
  }
  else if (!strcmp(str.content, "SET")) {
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
  else if (!strcmp(str.content, "PX")) {
    res.type = T_PX;
  }
  else if (is_number(str.content)) {
    res.type = T_NUM;
    res.value = value_number(atof(str.content));
  }
  else {
    res.type = T_STR;
    res.value = value_str(str_new(str.content));
  }
  //printf("type: %d; str: %s\n", res.type, str.content);
  return res;
}

TokenList tokenize(Str line)
{
  TokenList tokens = {0};
  DA_INIT(&tokens, Token);
  ssize_t len = 0;
  char word[WORD_LEN] = {0};
  char* wb = line.content;
  char sep = ' ';
  char quotes = 0;
  for (size_t i = 0; i < line.len; i++, len++) {
    if (line.content[i] == '"') quotes++;
    if (line.content[i] == sep && quotes%2 == 0) {
      if (len > 0) {
        strncpy(word, wb, len);
        word[len] = 0;
        DA_PUSH(&tokens, token_new((Str){.content = word, .len = len}));
      }
      while (line.content[i+1] == sep) i++;
      wb = &line.content[i+1];
      len = -1;
    }
  }
  if (len > 0) {
    strncpy(word, wb, len);
    word[len] = 0;
    DA_PUSH(&tokens, token_new((Str){.content = word, .len = len}));
  }
  return tokens;
}

ssize_t currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);
  ssize_t s1 = (ssize_t)(time.tv_sec) * 1000;
  ssize_t s2 = (time.tv_usec / 1000);
  return s1 + s2;
}

bool command_set(TokenList tokens) 
{
  if (tokens.items[0].type == T_SET) {
    char usage[] = "USAGE: SET <key> <value> [PX <milliseconds>]";
    assert(tokens.length > 1 && usage);
    int i = 1;
    Token key_tok = tokens.items[i];
    assert(key_tok.type == T_STR && "Expected key");
    Str key = str_value(key_tok.value);
    i++;
    assert((size_t)i < tokens.length && "Expected value");
    Token val_tok = tokens.items[i];
    Value val = val_tok.value;
    ht_insert(&db, key, val);
    if (tokens.length > 3) {
      i++;
      Token px_tok = tokens.items[i];
      assert(px_tok.type == T_PX && "Expected px");
      i++;
      assert((size_t)i < tokens.length && "Expected milliseconds");
      Token millis_tok = tokens.items[i];
      Value millis_val = millis_tok.value;
      millis_val.number += currentTimeMillis();
      ht_insert(&expirations, key, millis_val);
    }
    sprintf(response, "+OK\n");
    return true;
  }
  return false;
}

bool command_get(TokenList tokens)
{
  if (tokens.items[0].type == T_GET) {
    assert(tokens.length > 1 && "USAGE: GET <key>");
    int i = 1;
    Token key_tok = tokens.items[i];
    assert(key_tok.type == T_STR && "Expected key");
    Str key = str_value(key_tok.value);
    List values = ht_get(&db, key);;
    List expiration = ht_get(&expirations, key);
    if (expiration.head && currentTimeMillis() > expiration.head->value.number) {
      values = (List){0};
    }
    list_print(&values, response);
    return true;
  }
  return false;
}

bool command_ping(TokenList tokens)
{
  if (tokens.items[0].type == T_PING) {
    assert(tokens.length == 1 && "USAGE: PING");
    sprintf(response, "+PONG\n");
    return true;
  }
  return false;
}

char* interpret(TokenList tokens)
{
  bool did = command_set(tokens)
          || command_get(tokens)
          || command_ping(tokens);
  assert(did && "Command didn't recognized");
  return response;
}
