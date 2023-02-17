#include "Callable.h"

#include "Return.h"
#include <chrono>
#include <iostream>
#include <string>

using namespace std::chrono;
using std::string;

// Clock native function stuff
int ClockCallable::arity() { return 0; }

Obj ClockCallable::call(Interpreter &interpreter, list<Obj> arguments) {
  double time =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch())
          .count() /
      1000.0;
  return time;
}

string ClockCallable::to_string() { return "<native fn>"; }

// Function stuff
FunctionCallable::FunctionCallable(Function<Obj> *declaration)
    : declaration(declaration) {}

Obj FunctionCallable::call(Interpreter &interpreter, list<Obj> arguments) {
  auto env = new Environment(interpreter.globals);

  auto param_itr = declaration->params.begin();
  auto args_itr = arguments.begin();
  for (size_t i = 0; i < declaration->params.size(); i++) {
    env->define((*param_itr).lexeme, (*args_itr));
    param_itr++;
    args_itr++;
  }

  try {
    interpreter.executeBlock(declaration->body, env);
  } catch (ReturnError ret) {
    return ret.value;
  }

  return monostate();
}

int FunctionCallable::arity() { return declaration->params.size(); }

string FunctionCallable::to_string() {
  return "<fn " + declaration->name.lexeme;
}
