#include <stddef.h>
#include "parse_node.h"
#include "kernel_builder.h"
#include "token.h"

Node::Node(const char* name, Token *t) : name(name), children_count(0), token(t), parent(NULL) {}
Node::Node(const char* name) : name(name), children_count(0), token(NULL), parent(NULL) {}
Node::Node(Token *t, Node *parent) : name("Unamed"), children_count(0), token(t), parent(NULL) {}
Node::Node() : name("blank"), children_count(0), token(NULL), parent(NULL) {}
Node::~Node() {
  //delete allocated nodes
  while(children_count > 0) {
    delete children[children_count++];
  }
  delete children; //delete pointer array
}

size_t Node::numberOfChildren() {
  return this->children_count;
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

Node *Node::buildChild(const char *name) {
  Node *n = new Node(name);
  this->addChild(n);
  return n;
}

Node *Node::buildChild(const char *name, Token *t) {
  Node *n = new Node(name, t);
  this->addChild(n);
  return n;
}

void Node::print(KernelBuilder &b) {
  this->print(1, b);
}

void Node::print(int spaces, KernelBuilder &b) {
  for(int i = 0; i < spaces; ++i) {
    b.put("-");
  }
  b.put(name);
  if(this->children_count > 0) {
    b.put("(");
    b.putInt(this->children_count);
    b.put(")");
  }

  if(token != NULL) {
    b.put(" [");
    token->print(b);
    b.put("]");
  }
  b.put("", true);

  for(size_t i = 0; i < children_count; ++i) {
     children[i]->print(spaces+1, b);
  }
}