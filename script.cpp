#include "new.h"
#include "string_functions.h"
#include "script.h"

Token::Token(char c) : next(NULL), prev(NULL), str(new char), type(UNKNOWN) {
  str[0] = c;
  type = tok_type(c);
}

Token::Token(char c, Token *prev) : next(NULL), prev(prev), str(new char), type(UNKNOWN) {
  str[0] = c;
  type = tok_type(c);
}

Token::~Token() {
  delete str;
  delete next;
}

Token *Token::add(char c) {
  Type nextType = tok_type(c);
  if(nextType != type) {
    next = new Token(c, this);
    return next;
  }else{
    int len = strlen(str);
    const char *oldstr = str;
    str = new char[len+1];
    strcpy(str, oldstr);
    delete oldstr;
    str[len+1] = c;
    return this;
  }
}

Type Token::tok_type(char c) {
  if(c == ' ' || c == '\t') {
    return WHITESPACE;
  }else if(c == '\n') {
    return NEWLINE;
  }else if(c >= '0' || c <= '9') {
    return NUMBER;
  }else if( (c >= 'a' || c <= 'z') ||
            (c >= 'A' || c <= 'Z') ||
            (c == '_')) {
    return LETTER;
  }else if(c == '=' || c == '+' || c == '-' || c == '&' || c == '^' ||
           c == '|') {
    return OPERATOR;
  }else if( (c == '"' || c == '\'')) {
    return QUOTE;
  }else{
    return UNKNOWN;
  }
}
