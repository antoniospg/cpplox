#include "Environment.h"

#include <iostream>

#include "Interpreter.h"
using namespace std;

Environment::Environment() { enclosing = nullptr; }

Environment::Environment(Environment *enclosing) : enclosing(enclosing) {}

void Environment::assign(Token name, Obj val) {
  if (values.find(name.lexeme) != values.end()) {
    values[name.lexeme] = val;
    return;
  }

  // Search in outer scope
  if (enclosing) {
    enclosing->assign(name, val);
    return;
  }

  throw RuntimeError(name, string("Undefined variable '") + name.lexeme +
                               string("'."));
}

void Environment::define(string name, Obj val) { values[name] = val; }

Obj Environment::get(Token name) {
  if (values.find(name.lexeme) != values.end())
    return values[name.lexeme];

  // Search in outer scope
  if (enclosing)
    return enclosing->get(name);

  throw RuntimeError(name, string("Undefined variable '") + name.lexeme +
                               string("'."));
}

void Environment::clear() { values.clear(); }

void Environment::dumpValues() {
  for (auto [k, v] : values)
    cout << "env_dump " << k << endl;
}
