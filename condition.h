
#ifndef CONDITION_H_
#define CONDITION_H_

class Action;
class Context;

class Condition {
  public:
    Condition(Action *action);
    virtual ~Condition();

    bool check(Context *c);

    Action *getAction();
    
  private:
    Action *_first;
};

#endif