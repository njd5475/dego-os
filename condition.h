
#ifndef CONDITION_H_
#define CONDITION_H_

class Action;
class Context;

class Condition {

public:
  Condition();
  virtual ~Condition();

  virtual bool check(Context *c) = 0;
  private:
    Action *first;
};

#endif