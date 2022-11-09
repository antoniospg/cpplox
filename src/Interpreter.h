#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdexcept>
#include <string>

#include "Util.h"
#include "gen/Expr.hpp"
#include "gen/Stmt.hpp"

class RuntimeError : public runtime_error {
  public:
   Token token;
   RuntimeError(Token token, string message)
       : runtime_error(message), token(token) {}
};

class Interpreter : ExprAstVisitor<Obj>, StmtAstVisitor<Obj> {
  public:
   bool err;
   Interpreter();
   Obj visitExprBinary(Binary<Obj>* expr) override;
   Obj visitExprGrouping(Grouping<Obj>* expr) override;
   Obj visitExprLiteral(Literal<Obj>* expr) override;
   Obj visitExprUnary(Unary<Obj>* expr) override;
   Obj visitStmtExpression(Expression<Obj>* expr) override;
   Obj visitStmtPrint(Print<Obj>* expr) override;
   void execute(Stmt<Obj>* stmt);
   void interpret(vector<Stmt<Obj>*> expr);

  private:
   Obj evaluate(Expr<Obj>* expr);
   bool isTrue(Obj value);
   bool isEqual(Obj v1, Obj v2);
   void checkNumberOperand(Token token, Obj operand);
   void checkNumberOperand(Token token, Obj left, Obj right);
   void runtimeError(const RuntimeError& err);
};

#endif
