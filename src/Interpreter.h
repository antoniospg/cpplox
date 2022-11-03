#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

#include "Util.h"
#include "gen/Expr.hpp"

class Interpreter : ExprAstVisitor<Obj> {
 public:
  Obj visitExprBinary(Binary<Obj>* expr) override;
  Obj visitExprGrouping(Grouping<Obj>* expr) override;
  Obj visitExprLiteral(Literal<Obj>* expr) override;
  Obj visitExprUnary(Unary<Obj>* expr) override;
};

#endif
