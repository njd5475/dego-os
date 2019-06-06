
#ifndef _NODE_VISITOR_H_
#define _NODE_VISITOR_H_

class Node;
class Program;

class NodeVisitor {
  public:
    NodeVisitor(Program *program) : _program(program) {}
    virtual ~NodeVisitor() {}

    virtual void visit(Node *node);

  private:
    Program *_program;
};

#endif