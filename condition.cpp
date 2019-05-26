
#include "condition.h"
#include "context.h"
#include "action.h"

Condition::Condition(Action *action) : _first(action) {

}

Condition::~Condition() {

}

Action *Condition::getAction() {
  return _first;
}

bool Condition::check(Context *context) {
  return true;
}