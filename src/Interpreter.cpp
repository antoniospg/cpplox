#include "Interpreter.h"

#include <iostream>
#include <map>
#include <string>
#include <variant>

#include "Callable.h"

using namespace std;

Interpreter::Interpreter() : err(false) {
  // Initialize environments
  globals = new Environment();
  env = globals;

  globals->define("clock", new ClockCallable());
}

Obj Interpreter::visitExprAssign(Assign<Obj> *expr) {
  Obj value = evaluate(expr->value);
  env->assign(expr->name, value);

  return value;
}

Obj Interpreter::visitExprLiteral(Literal<Obj> *expr) { return expr->value; }

Obj Interpreter::visitExprLogical(Logical<Obj> *expr) {
  Obj left = evaluate(expr->left);

  if (expr->op.type == OR) {
    if (isTrue(left)) return left;
  } else {
    if (!isTrue(left)) return left;
  }

  return evaluate(expr->right);
}

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

Obj Interpreter::visitStmtBlock(Block<Obj> *stmt) {
  executeBlock(stmt->statements, new Environment(env));
  return monostate();
}

Obj Interpreter::visitExprVariable(Variable<Obj> *expr) {
  return env->get(expr->name);
}

Obj Interpreter::visitExprCall(Call<Obj> *expr) {
  auto callee = evaluate(expr->callee);

  list<Obj> arguments;
  for (auto arg : expr->arguments) arguments.push_back(evaluate(arg));

  if (!holds_alternative<Callable *>(callee))
    throw RuntimeError(expr->paren, "Can only call functions and classes.");

  Callable *function = get<Callable *>(callee);
  if (arguments.size() != function->arity())
    throw RuntimeError(expr->paren,
                       to_string("Expected ") + to_string(function->arity()) +
                           to_string(" arguments but got ") +
                           to_string(arguments.size()) + to_string('.'));
  return function->call(*this, arguments);
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
    case EQUAL_EQUAL:
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

Obj Interpreter::visitStmtFunction(Function<Obj> *stmt) {
  FunctionCallable *function = new FunctionCallable(stmt);
  env->define(stmt->name.lexeme, function);
  return monostate();
}

Obj Interpreter::visitStmtIf(If<Obj> *stmt) {
  if (isTrue(evaluate(stmt->condition))) {
    execute(stmt->thenBranch);
  } else if (stmt->elseBranch) {
    execute(stmt->elseBranch);
  }

  return monostate();
}

Obj Interpreter::visitStmtPrint(Print<Obj> *stmt) {
  auto value = evaluate(reinterpret_cast<Expr<Obj> *>(stmt->expr));
  cout << to_string(value) << endl;
  return monostate();
}

Obj Interpreter::visitStmtVar(Var<Obj> *stmt) {
  Obj val = monostate();
  if (stmt->initializer != nullptr) val = evaluate(stmt->initializer);

  env->define(stmt->name.lexeme, val);
  return monostate();
}

Obj Interpreter::visitStmtWhile(While<Obj> *stmt) {
  while (isTrue(evaluate(stmt->condition))) execute(stmt->body);

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
  if (holds_alternative<double>(operand)) return;
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

void Interpreter::executeBlock(list<Stmt<Obj> *> stmts, Environment *env) {
  Environment *previous = this->env;
  try {
    this->env = env;

    for (auto s : stmts) execute(s);
  } catch (const RuntimeError &err) {
    this->env = previous;
  }
}

void Interpreter::interpret(vector<Stmt<Obj> *> statements) {
  try {
    for (auto &statement : statements) execute(statement);
  } catch (const RuntimeError &err) {
    runtimeError(err);
  }
}

void Interpreter::runtimeError(const RuntimeError &error) {
  cout << "[line " + to_string(error.token.line) + "] " << error.what() << endl;
  err = true;
}
