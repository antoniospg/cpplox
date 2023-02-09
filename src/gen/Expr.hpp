#ifndef EXPR_HPP
#define EXPR_HPP
#include <string>
#include <memory>
#include "../Lex.h"
#include "../Util.h"
using namespace std;

template <typename T>
class Binary;

template <typename T>
class Grouping;

template <typename T>
class Literal;

template <typename T>
class Unary;

template <typename T>
class Variable;

template <typename T>
class Assign;

template <typename T>
class Expr;

template <typename T>
class ExprAstVisitor {
public:
  virtual T visitExprBinary (Binary<T>* expr) = 0;
  virtual T visitExprGrouping (Grouping<T>* expr) = 0;
  virtual T visitExprLiteral (Literal<T>* expr) = 0;
  virtual T visitExprUnary (Unary<T>* expr) = 0;
  virtual T visitExprVariable (Variable<T>* expr) = 0;
  virtual T visitExprAssign (Assign<T>* expr) = 0;
};

template <typename T>
class Expr {
  public:
  Expr( )  {}
  virtual T accept (ExprAstVisitor<T>* visitor) = 0;
};

template <typename T>
class Binary : public Expr<T> {
  public:
  Expr<T> * left;
  Token op;
  Expr<T> * right;
  Binary( Expr<T> * left, Token op, Expr<T> * right) : left(left), op(op), right(right) {}
  T accept (ExprAstVisitor<T>* visitor) {
    return visitor->visitExprBinary(this);
  }
};

template <typename T>
class Grouping : public Expr<T> {
  public:
  Expr<T> * grouping;
  Grouping( Expr<T> * grouping) : grouping(grouping) {}
  T accept (ExprAstVisitor<T>* visitor) {
    return visitor->visitExprGrouping(this);
  }
};

template <typename T>
class Literal : public Expr<T> {
  public:
  Obj value;
  Literal( Obj value) : value(value) {}
  T accept (ExprAstVisitor<T>* visitor) {
    return visitor->visitExprLiteral(this);
  }
};

template <typename T>
class Unary : public Expr<T> {
  public:
  Token op;
  Expr<T> * right;
  Unary( Token op, Expr<T> * right) : op(op), right(right) {}
  T accept (ExprAstVisitor<T>* visitor) {
    return visitor->visitExprUnary(this);
  }
};

template <typename T>
class Variable : public Expr<T> {
  public:
  Token name;
  Variable( Token name) : name(name) {}
  T accept (ExprAstVisitor<T>* visitor) {
    return visitor->visitExprVariable(this);
  }
};

template <typename T>
class Assign : public Expr<T> {
  public:
  Token name;
  Expr<T> * value;
  Assign( Token name, Expr<T> * value) : name(name), value(value) {}
  T accept (ExprAstVisitor<T>* visitor) {
    return visitor->visitExprAssign(this);
  }
};
#endif
