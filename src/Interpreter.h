#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <list>
#include <stdexcept>
#include <string>

#include "Environment.h"
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
  Environment *globals;
  Interpreter();
  Obj visitExprAssign(Assign<Obj> *expr) override;
  Obj visitExprBinary(Binary<Obj> *expr) override;
  Obj visitExprCall(Call<Obj> *expr) override;
  Obj visitExprGrouping(Grouping<Obj> *expr) override;
  Obj visitExprLiteral(Literal<Obj> *expr) override;
  Obj visitExprLogical(Logical<Obj> *expr) override;
  Obj visitExprUnary(Unary<Obj> *expr) override;
  Obj visitExprVariable(Variable<Obj> *expr) override;
  Obj visitStmtBlock(Block<Obj> *stmt) override;
  Obj visitStmtExpression(Expression<Obj> *expr) override;
  Obj visitStmtFunction(Function<Obj> *expr) override;
  Obj visitStmtIf(If<Obj> *stmt) override;
  Obj visitStmtPrint(Print<Obj> *expr) override;
  Obj visitStmtReturn(Return<Obj> *expr) override;
  Obj visitStmtVar(Var<Obj> *stmt) override;
  Obj visitStmtWhile(While<Obj> *stmt) override;
  void execute(Stmt<Obj> *stmt);
  void executeBlock(list<Stmt<Obj> *> stmts, Environment *env);
  void interpret(vector<Stmt<Obj> *> expr);

private:
  Environment *env;
  Obj evaluate(Expr<Obj> *expr);
  void assign(Token name, Obj value);
  bool isTrue(Obj value);
  bool isEqual(Obj v1, Obj v2);
  void checkNumberOperand(Token token, Obj operand);
  void checkNumberOperand(Token token, Obj left, Obj right);
  void runtimeError(const RuntimeError &err);
};

#endif
