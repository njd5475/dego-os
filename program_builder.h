
#ifndef PROGRAM_BUILDER_H_
#define PROGRAM_BUILDER_H_

#include "token.h"
#include "parser.h"
#include "program.h"
#include "parse_node.h"

class ProgramBuilder {

public:
  ProgramBuilder();
  ~ProgramBuilder();

  Program *build(Node *ast);
private:
  Program *_program;
};

#endif