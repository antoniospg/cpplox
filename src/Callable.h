#ifndef CALLABLE_H
#define CALLABLE_H

#include "Interpreter.h"
#include "Util.h"

class Callable {
public:
  virtual int arity() = 0;
  virtual Obj call(Interpreter &interpreter, list<Obj> arguments) = 0;
  virtual string to_string() = 0;
};

class ClockCallable : public Callable {
  int arity() override;
  Obj call(Interpreter &interpreter, list<Obj> arguments) override;
  string to_string() override;
};

class FunctionCallable : public Callable {
public:
  int arity() override;
  Obj call(Interpreter &interpreter, list<Obj> arguments) override;
  string to_string() override;
  FunctionCallable(Function<Obj> *declaration);

private:
  Function<Obj> *declaration;
};

#endif
