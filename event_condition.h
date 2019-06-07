
#ifndef _EVENT_CONDITION_H_
#define _EVENT_CONDITION_H_

#include "condition.h"

class EventCondition : public Condition {

  public:
    EventCondition(const char *name, Action *action) : Condition(action), _name(name) {}
    virtual ~EventCondition() {}

    virtual bool check(Context *context);
  private:
    const char *_name;
};

#endif