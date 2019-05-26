#ifndef _PROGRAM_H_
#define _PROGRAM_H_

class Context;
class Condition;

class Program {
  public:
    Program();
    ~Program();

    void exec(Context *context);
    
  private:
    Condition **conditions;
};

#endif