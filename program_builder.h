
#ifndef PROGRAM_BUILDER_H_
#define PROGRAM_BUILDER_H_

class Program;
class Node;

class ProgramBuilder {

public:
  ProgramBuilder();
  ~ProgramBuilder();

  Program *build(Node *ast);
};

#endif