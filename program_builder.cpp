
#include "program_builder.h"

#include "context.h"
#include "parser.h"
#include "parse_node.h"

ProgramBuilder::ProgramBuilder() : _program(new Program()) {
  //constructor
}

ProgramBuilder::~ProgramBuilder() {
  // destructor
}

Program *ProgramBuilder::build(Node *ast) { 
  Node *current;
  
  //tree traversal
  current = ast;
  

  return this->_program;
}