
#include "program_builder.h"

#include "kernel_builder.h"
#include "context.h"
#include "parser.h"
#include "parse_node.h"

ProgramBuilder::ProgramBuilder() : _program(new Program()), _terminal(new KernelBuilder()), NodeVisitor(_program), _lastAction(0) {
  //constructor
  _terminal->put("Hello From Program Builder");
}

ProgramBuilder::~ProgramBuilder() {
  // destructor
}

Program *ProgramBuilder::build(Node *ast) {
  _terminal->putln("Calling builder");
  ast->visit(this);

  return this->_program;
}

void ProgramBuilder::visit(Node *n) {
  Condition *cond = 0;
  bool condComplete = false;
  
  if(strcmp("printCommand", n->getName())) {
    _terminal->put("Got print command");
  }else if(strcmp("conditions", n->getName())) {
    if(condComplete) {
      
    }

    if(n->numberOfChildren() > 0 && n->getChild(0)->isToken("event")) {

    }
  }else if(strcmp("action", n->getName())) {

  }else{
    // _terminal->put("Got unprocessed Node:");
    // _terminal->put(n->getName());
    // _terminal->put(" ");
  }
}