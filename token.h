
#ifndef TOKEN_H_
#define TOKEN_H_

enum Type {
    UNKNOWN = 0,
    NUMBER,
    LETTER,
    OPERATOR,
    WHITESPACE,
    NEWLINE,
    QUOTE,
    AT_SYMBOL,
    DOT
};

class KernelBuilder;

class Token {
public:
  Token(char c);
  Token(char c, Token *tok);
  ~Token();

  Token *add(char c);
  Token *next() {return _next;}
  const char *tok() {return _str;}
  Type ttype() {return _type;}
  void print(KernelBuilder &b);
private:
  Type tok_type(char c);

  Type _type;
  char *_str;
  Token *_next;
  Token *_prev;
};

#endif