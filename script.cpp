#include "new.h"
#include "string_functions.h"
#include "script.h"
#include "terminal.h"
#include "kernel_builder.h"

Terminal term;

Token::Token(char c) : _next(NULL), _prev(NULL), _str(new char), _type(UNKNOWN) {
  _str[0] = c;
  _type = tok_type(c);
}

Token::Token(char c, Token *prev) : _next(NULL), _prev(prev), _str(new char), _type(UNKNOWN) {
  _str[0] = c;
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
    _str = new char[len+1];
    strcpy(_str, oldstr);
    delete oldstr;
    _str[len+1] = c;
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

Node::Node(Token *t) : token(t), parent(NULL) {
  term.printLine("root:Node");
}

Node::Node(Token *t, Node *parent) : token(t), parent(NULL) {
  term.printLine("child:Node");
}

Node::Node() : token(NULL), parent(NULL) {

}

Node::~Node() {
  //delete allocated nodes
  while(children_count > 0) {
    delete children[children_count++];
  }
  delete children; //delete pointer array
}

void Node::addChild(Node *node) {
  Node **new_children = new Node*[children_count+1];
  //copy
  for(size_t i = 0; i < children_count; ++i) {
    new_children[i] = children[i];
  }
  new_children[children_count] = node;
  ++children_count;
  delete []children;
  children = new_children;
}

void Node::print(KernelBuilder *b) {
  b->put("node");
  // for(size_t i = 0; i < children_count; ++i) {
  //   children[i]->print(b);
  // }
}

Token *whenz(Token *, Node *n);
Token *keyword(const char *word, Token *t, Node *current);
Token *conditions(Token *t, Node *current);
Token *actions(Token *t, Node *current);
Token *identifier(Token *t, Node *current);

Node *buildAST(Token *head) {
  term.printLine("buildAST");
  Node *ast = new Node();
  //head = whenz(head, ast);
  term.printLine("done");
  return ast;
}

Token *whenz(Token *tok, Node *current) {
  tok = keyword("when", tok, current);
  tok = conditions(tok, current);
  tok = keyword("do", tok, current);
  tok = actions(tok, current);
}

Token *keyword(const char *word, Token *t, Node *n) {
  if(strcmp(word, t->value())) {
    n->addChild(new Node(t));
    return t->next();
  }
  return t->next();
}

Token *conditions(Token *t, Node *n) {
  Node *node = new Node();
  n->addChild(node);
  t = identifier(t, node);
}

Token *identifier(Token *t, Node *n) {
  if(t->ttype() == LETTER || t->ttype() == NUMBER) {
    t = t->next();
    n->addChild(new Node(t));
  }
  return t;
}

Token *actions(Token *t, Node *n) {

}
