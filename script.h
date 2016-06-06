#ifndef _script_h_
#define _script_h_ 1

enum Type {
    UNKNOWN = 0,
    NUMBER,
    LETTER,
    OPERATOR,
    WHITESPACE,
    NEWLINE,
    QUOTE
};

class Token {
public:
  Token(char c);
  Token(char c, Token *tok);
  ~Token();

  Token *add(char c);
private:
  Type tok_type(char c);

  Type type;
  char *str;
  Token *next;
  Token *prev;
};

#endif
