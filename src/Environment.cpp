#include "Environment.h"
#include "Interpreter.h"

#include <iostream>
using namespace std;

void Environment::define(string name, Obj val) { values[name] = val; }

Obj Environment::get(Token name) {
  if (values.find(name.lexeme) != values.end())
    return values[name.lexeme];

  throw RuntimeError(name, string("Undefined variable '") + name.lexeme +
                               string("'."));
}

void Environment::clear() { values.clear(); }
