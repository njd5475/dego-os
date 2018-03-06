#ifndef _script_h_
#define _script_h_ 1

class KernelBuilder;

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

class Node {
public:
  Node(const char *name, Token *t);
  Node(const char *name);
  Node();
  ~Node();

  bool hasToken() {return !token;}
  void addChild(Node *);
  void print(KernelBuilder &);
  void print(int, KernelBuilder &);
protected:
  Node(Token *, Node *);

private:
  const char *name;
  Node *parent;
  Node **children;
  size_t children_count;
  Token *token;
};

class Parser {
public:
  Parser(KernelBuilder &b);
  ~Parser();

  Node *parse(Token *head);


  Token *whenz(Token *, Node *n);
  Token *keyword(const char *word, Token *t, Node *current);
  Token *conditions(Token *t, Node *current);
  Token *actions(Token *t, Node *current);
  Token *identifier(Token *t, Node *current);
  Token *nullToken(Node *current);
  Token *consumeWhitespace(Token *t);
  Token *unknownToken(Token *t);

private:
  KernelBuilder &_b;
};

#endif
