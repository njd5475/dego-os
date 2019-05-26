
#include "string_functions.h"
#include "token.h"
#include "kernel_builder.h"

Token::Token(char c) : _next(NULL), _prev(NULL), _str(new char[2]), _type(UNKNOWN) {
  _str[0] = c;
  _str[1] = '\0';
  _type = tok_type(c);
}

Token::Token(char c, Token *prev) : _next(NULL), _prev(prev), _str(new char[2]), _type(UNKNOWN) {
  _str[0] = c;
  _str[1] = '\0';
  _type = tok_type(c);
}

Token::~Token() {
  delete _str;
  delete _next;
}

Token *Token::add(char c) {
  Type nextType = tok_type(c);
  if(nextType != ttype()) {
    _next = new Token(c, this);
    return _next;
  }else{
    int len = strlen(_str);
    const char *oldstr = _str;
    _str = new char[len+2];
    strcpy(_str, oldstr);
    delete oldstr;
    _str[len] = c;
    _str[len+1] = '\0';
    return this;
  }
}

Type Token::tok_type(char c) {
  if(c == ' ' || c == '\t') {
    return WHITESPACE;
  }else if(c == '\n') {
    return NEWLINE;
  }else if(c >= '0' && c <= '9') {
    return NUMBER;
  }else if( (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == '_')) {
    return LETTER;
  }else if(c == '=' || c == '+' || c == '-' || c == '&' || c == '^' ||
           c == '|') {
    return OPERATOR;
  }else if(c == '@') {
    return AT_SYMBOL;
  }else if(c == '.') {
    return DOT;
  }else if(c == '"' || c == '\'') {
    return QUOTE;
  }else{
    return UNKNOWN;
  }
}

void Token::print(KernelBuilder &b) {
  switch(_type) {
    case NUMBER:
      b.put("NUMBER");
      break;
    case LETTER:
      b.put("LETTER");
      break;
    case OPERATOR:
      b.put("OPERATOR");
      break;
    case WHITESPACE:
      b.put("WHITESPACE");
      break;
    case NEWLINE:
      b.put("NEWLINE");
      break;
    case QUOTE:
      b.put("QUOTE");
      break;
    case AT_SYMBOL:
      b.put("AT_SYM");
      break;
    case DOT:
      b.put("DOT");
      break;
    case UNKNOWN:
    default:
      b.put("UNK[");
      unsigned short len = strlen(this->_str);
      b.putInt(len);
      b.put("]");
      break;
  }
  b.put(":");
  b.put(this->tok());
}