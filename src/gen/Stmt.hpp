#ifndef STMT_HPP
#define STMT_HPP
#include <string>
#include <memory>
#include <list>
#include "../Lex.h"
#include "../Util.h"
#include "Expr.hpp"
using namespace std;

template <typename T>
class Expression;

template <typename T>
class Function;

template <typename T>
class If;

template <typename T>
class Print;

template <typename T>
class Return;

template <typename T>
class Var;

template <typename T>
class While;

template <typename T>
class Block;

template <typename T>
class Stmt;

template <typename T>
class StmtAstVisitor {
public:
  virtual T visitStmtExpression (Expression<T>* stmt) = 0;
  virtual T visitStmtFunction (Function<T>* stmt) = 0;
  virtual T visitStmtIf (If<T>* stmt) = 0;
  virtual T visitStmtPrint (Print<T>* stmt) = 0;
  virtual T visitStmtReturn (Return<T>* stmt) = 0;
  virtual T visitStmtVar (Var<T>* stmt) = 0;
  virtual T visitStmtWhile (While<T>* stmt) = 0;
  virtual T visitStmtBlock (Block<T>* stmt) = 0;
};

template <typename T>
class Stmt {
  public:
  Stmt( )  {}
  virtual T accept (StmtAstVisitor<T>* visitor) = 0;
};

template <typename T>
class Expression : public Stmt<T> {
  public:
  Expr<T> * expr;
  Expression( Expr<T> * expr) : expr(expr) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtExpression(this);
  }
};

template <typename T>
class Function : public Stmt<T> {
  public:
  Token name;
  list<Token> params;
  list<Stmt<T> *> body;
  Function( Token name, list<Token> params, list<Stmt<T> *> body) : name(name), params(params), body(body) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtFunction(this);
  }
};

template <typename T>
class If : public Stmt<T> {
  public:
  Expr<T> * condition;
  Stmt<T> * thenBranch;
  Stmt<T> * elseBranch;
  If( Expr<T> * condition, Stmt<T> * thenBranch, Stmt<T> * elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtIf(this);
  }
};

template <typename T>
class Print : public Stmt<T> {
  public:
  Expr<T> * expr;
  Print( Expr<T> * expr) : expr(expr) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtPrint(this);
  }
};

template <typename T>
class Return : public Stmt<T> {
  public:
  Token keyword;
  Expr<T> * value;
  Return( Token keyword, Expr<T> * value) : keyword(keyword), value(value) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtReturn(this);
  }
};

template <typename T>
class Var : public Stmt<T> {
  public:
  Token name;
  Expr<T> * initializer;
  Var( Token name, Expr<T> * initializer) : name(name), initializer(initializer) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtVar(this);
  }
};

template <typename T>
class While : public Stmt<T> {
  public:
  Expr<T> * condition;
  Stmt<T> * body;
  While( Expr<T> * condition, Stmt<T> * body) : condition(condition), body(body) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtWhile(this);
  }
};

template <typename T>
class Block : public Stmt<T> {
  public:
  list<Stmt<T> *> statements;
  Block( list<Stmt<T> *> statements) : statements(statements) {}
  T accept (StmtAstVisitor<T>* visitor) {
    return visitor->visitStmtBlock(this);
  }
};
#endif
