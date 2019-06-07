
#ifndef PROGRAM_BUILDER_H_
#define PROGRAM_BUILDER_H_

#include "token.h"
#include "parser.h"
#include "program.h"
#include "parse_node.h"
#include "node_visitor.h"

class KernelBuilder;
class Action;

class ProgramBuilder: public NodeVisitor {

public:
  ProgramBuilder();
  virtual ~ProgramBuilder();

  Program *build(Node *ast);
  virtual void visit(Node *n);

private:
  Program *_program;
  KernelBuilder *_terminal;
  Action *_lastAction;
};

#endif