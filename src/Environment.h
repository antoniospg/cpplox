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

public:
  void define(string name, Obj val);
  Obj get(Token name);
  void clear();
};

#endif
