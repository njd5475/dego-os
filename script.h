#ifndef _script_h_
#define _script_h_ 1

class KernelBuilder;
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
  Token *next() {return _next;}
  const char *value() {return _str;}
  Type ttype() {return _type;}
private:
  Type tok_type(char c);

  Type _type;
  char *_str;
  Token *_next;
  Token *_prev;
};

class Node {
public:
  Node(Token *t);
  Node();
  ~Node();

  bool hasToken() {return !token;}
  void addChild(Node *);
  void print(KernelBuilder *);
protected:
  Node(Token *, Node *);

private:
  Node *parent;
  Node **children;
  size_t children_count;
  Token *token;
};

Node *buildAST(Token *);

#endif
