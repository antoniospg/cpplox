#include "Environment.h"

#include <iostream>

#include "Interpreter.h"
using namespace std;

void Environment::assign(Token name, Obj val) {
  if (values.find(name.lexeme) != values.end()) {
    values[name.lexeme] = val;
    return;
  }

  throw RuntimeError(
      name, string("Undefined variable '") + name.lexeme + string("'."));
}

void Environment::define(string name, Obj val) { values[name] = val; }

Obj Environment::get(Token name) {
  if (values.find(name.lexeme) != values.end()) return values[name.lexeme];

  throw RuntimeError(
      name, string("Undefined variable '") + name.lexeme + string("'."));
}

void Environment::clear() { values.clear(); }
