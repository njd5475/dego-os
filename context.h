#ifndef __CONTEXT_H_
#define __CONTEXT_H_ 1

class Condition;

class Context {

  public:
    Context();
    ~Context();

  private:

};

class Program {
  public:
    Program();
    ~Program();

  private:
    Condition **conditions;
};

#endif
