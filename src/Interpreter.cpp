#include "Interpreter.h"

#include <iostream>
#include <map>
#include <string>
#include <variant>

using namespace std;

Environment Interpreter::env;

Interpreter::Interpreter() : err(false) {
  // Clear static field env
  // env.clear();
}

Obj Interpreter::visitExprLiteral(Literal<Obj> *expr) { return expr->value; }

Obj Interpreter::visitExprGrouping(Grouping<Obj> *expr) {
  return evaluate(expr->grouping);
}

Obj Interpreter::visitExprUnary(Unary<Obj> *expr) {
  Obj right = evaluate(expr->right);

  switch (expr->op.type) {
  case MINUS:
    checkNumberOperand(expr->op, right);
    return -get<double>(right);
  case BANG:
    return isTrue(right);
  default:
    break;
  }

  return monostate();
}

Obj Interpreter::visitExprVariable(Variable<Obj> *expr) {
  return env.get(expr->name);
}

Obj Interpreter::visitExprBinary(Binary<Obj> *expr) {
  Obj left = evaluate(expr->left);
  Obj right = evaluate(expr->right);

  switch (expr->op.type) {
  case GREATER:
    checkNumberOperand(expr->op, left, right);
    return get<double>(left) > get<double>(right);
  case GREATER_EQUAL:
    checkNumberOperand(expr->op, left, right);
    return get<double>(left) >= get<double>(right);
  case LESS:
    checkNumberOperand(expr->op, left, right);
    return get<double>(left) < get<double>(right);
  case LESS_EQUAL:
    checkNumberOperand(expr->op, left, right);
    return get<double>(left) <= get<double>(right);
  case EQUAL:
    checkNumberOperand(expr->op, left, right);
    return isEqual(left, right);
  case BANG_EQUAL:
    checkNumberOperand(expr->op, left, right);
    return !isEqual(left, right);
  case PLUS:
    if (holds_alternative<double>(left) && holds_alternative<double>(right))
      return get<double>(left) + get<double>(right);

    if (holds_alternative<string>(left) && holds_alternative<string>(right))
      return get<string>(left) + get<string>(right);

    throw RuntimeError(expr->op, "Operands must be two numbers or strings");

    break;
  case MINUS:
    checkNumberOperand(expr->op, left, right);
    return get<double>(left) - get<double>(right);
  case SLASH:
    checkNumberOperand(expr->op, left, right);
    return get<double>(left) / get<double>(right);
  case STAR:
    checkNumberOperand(expr->op, left, right);
    return get<double>(left) * get<double>(right);
  default:
    break;
  }

  return monostate();
}

Obj Interpreter::visitStmtExpression(Expression<Obj> *stmt) {
  evaluate(reinterpret_cast<Expr<Obj> *>(stmt->expr));
  return monostate();
}

Obj Interpreter::visitStmtPrint(Print<Obj> *stmt) {
  auto value = evaluate(reinterpret_cast<Expr<Obj> *>(stmt->expr));
  cout << to_string(value) << endl;
  return monostate();
}

Obj Interpreter::visitStmtVar(Var<Obj> *stmt) {
  Obj val = monostate();
  if (stmt->initializer != nullptr)
    val = evaluate(stmt->initializer);

  env.define(stmt->name.lexeme, val);
  return monostate();
}

Obj Interpreter::evaluate(Expr<Obj> *expr) { return expr->accept(this); }

bool Interpreter::isTrue(Obj value) {
  if (holds_alternative<monostate>(value))
    return false;
  else if (holds_alternative<bool>(value))
    return get<bool>(value);
  else
    return true;
}

void Interpreter::checkNumberOperand(Token token, Obj operand) {
  if (holds_alternative<double>(operand))
    return;
  throw RuntimeError(token, "Operand must be a number!");
}

void Interpreter::checkNumberOperand(Token token, Obj left, Obj right) {
  checkNumberOperand(token, left);
  checkNumberOperand(token, right);
}

bool Interpreter::isEqual(Obj v1, Obj v2) {
  if (holds_alternative<monostate>(v1) && holds_alternative<monostate>(v2))
    return true;
  else if (holds_alternative<monostate>(v1))
    return false;
  else
    return v1 == v2;
}

void Interpreter::execute(Stmt<Obj> *stmt) { stmt->accept(this); }

void Interpreter::interpret(vector<Stmt<Obj> *> statements) {
  try {
    for (auto &statement : statements)
      execute(statement);
  } catch (const RuntimeError &err) {
    runtimeError(err);
  }
}

void Interpreter::runtimeError(const RuntimeError &error) {
  cout << "[line " + to_string(error.token.line) + "] " << error.what() << endl;
  err = true;
}
