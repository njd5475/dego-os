#ifndef PARSE_NODE_H_
#define PARSE_NODE_H_

#include "string_functions.h"
class Token;
class KernelBuilder;

class Node {
public:
  Node(const char *name, Token *t);
  Node(const char *name);
  Node();
  ~Node();

  bool hasToken() {return !token;}
  void addChild(Node *);
  Node *buildChild(const char *name);
  Node *buildChild(const char *name, Token *t);
  void print(KernelBuilder &);
  void print(int, KernelBuilder &);
  size_t numberOfChildren();
protected:
  Node(Token *, Node *);

private:
  const char *name;
  Node *parent;
  Node **children;
  size_t children_count;
  Token *token;
};


#endif