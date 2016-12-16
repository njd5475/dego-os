#include "new.h"
#include "string_functions.h"
#include "script.h"
#include "terminal.h"
#include "kernel_builder.h"

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
  }else if(c >= '0' && c <= '9') {
    return NUMBER;
  }else if( (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == '_')) {
    return LETTER;
  }else if(c == '=' || c == '+' || c == '-' || c == '&' || c == '^' ||
           c == '|') {
    return OPERATOR;
  }else if(c == '"' || c == '\'') {
    return QUOTE;
  }else{
    return UNKNOWN;
  }
}

void Token::print(KernelBuilder *b) {
  switch(_type) {
    case NUMBER:
      b->put("NUMBER");
      break;
    case LETTER:
      b->put("LETTER");
      break;
    case OPERATOR:
      b->put("OPERATOR");
      break;
    case WHITESPACE:
      b->put("WHITESPACE");
      break;
    case NEWLINE:
      b->put("NEWLINE");
      break;
    case QUOTE:
      b->put("QUOTE");
      break;
    case UNKNOWN:
    default:
      b->put("UNKNOWN");
      break;
  }
}

Node::Node(Token *t) : children_count(0), token(t), parent(NULL) {}
Node::Node(Token *t, Node *parent) : children_count(0), token(t), parent(NULL) {}
Node::Node() : children_count(0), token(NULL), parent(NULL) {}
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
  b->put("> ");
  if(token != NULL) {
    token->print(b);
  }
  for(size_t i = 0; i < children_count; ++i) {
     children[i]->print(b);
  }
}

/* Start of Parser */
Parser::Parser(KernelBuilder *b) : _b(b) {}
Parser::~Parser() {}

Node *Parser::parse(Token *head) {
  Node *n = new Node();
  while(head != NULL) {
    head = whenz(head, n);
  }
  _b->put("done");
  return n;
}

Token *Parser::whenz(Token *tok, Node *current) {
  tok = keyword("when", tok, current);
  if(!tok) return nullToken(current);
  tok = conditions(tok, current);
  if(!tok) return NULL;
  tok = keyword("do", tok, current);
  if(!tok) return NULL;
  tok = actions(tok, current);
  if(!tok) return NULL;
  return tok;
}

Token *Parser::keyword(const char *word, Token *t, Node *n) {
  t = consumeWhitespace(t);
  if(strcmp(word, t->value())) {
    n->addChild(new Node(t));
    return t->next();
  }else{
    unknownToken(t);
    return NULL;
  }
}

Token *Parser::conditions(Token *t, Node *n) {
  Node *node = new Node();
  t = identifier(t, node);
  if(t != NULL) {
    n->addChild(node);
  }else{
    nullToken(n);
  }
}

Token *Parser::identifier(Token *t, Node *n) {
  t = consumeWhitespace(t);
  if(t->ttype() == LETTER || t->ttype() == NUMBER) {
    t = t->next();
    n->addChild(new Node(t));
  }
  return t;
}

Token *Parser::consumeWhitespace(Token *t) {
  while(t->ttype() == WHITESPACE) {
    t = t->next();
  }
  return t;
}

Token *Parser::actions(Token *t, Node *n) {

}

Token *Parser::unknownToken(Token *t) {
  _b->put("Unknown Token");
  _b->put(t->value());
  return NULL;
}

Token *Parser::nullToken(Node *current) {
  _b->put("Unexpected NULL token!");
  return NULL;
}
