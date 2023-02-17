#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Lex.h"
#include "Util.h"
#include <map>
#include <string>

using namespace std;

class Environment {
private:
  map<string, Obj> values;
  Environment *enclosing;

public:
  Environment();
  Environment(Environment *enclosing);
  void dumpValues();
  void define(string name, Obj val);
  void assign(Token name, Obj value);
  Obj get(Token name);
  void clear();
};

#endif
