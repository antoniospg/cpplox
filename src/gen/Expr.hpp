#ifndef EXPR_HPP
#define EXPR_HPP
#include <string>
#include <memory>
#include "../Lex.h"
using namespace std;

class Binary;
class Grouping;
class Literal;
class Unary;
class Expr;

template <typename T>
class ExprAstVisitor {
  T visitExprBinary (Binary* expr);
  T visitExprGrouping (Grouping* expr);
  T visitExprLiteral (Literal* expr);
  T visitExprUnary (Unary* expr);
};

class Expr {
  public:
  Expr( )  {}
  template <typename T>
  void accept (ExprAstVisitor<T> visitor);
};

class Binary : Expr {
  public:
  Expr* left;
  Token op;
  Expr* right;
  Binary( Expr* left, Token op, Expr* right) : left(left), op(op), right(right) {}
  template <typename T>
  void accept (ExprAstVisitor<T> visitor) {
    visitor.visitBinary(this);
  }
};

class Grouping : Expr {
  public:
  Expr* grouping;
  Grouping( Expr* grouping) : grouping(grouping) {}
  template <typename T>
  void accept (ExprAstVisitor<T> visitor) {
    visitor.visitGrouping(this);
  }
};

class Literal : Expr {
  public:
  string value;
  Literal( string value) : value(value) {}
  template <typename T>
  void accept (ExprAstVisitor<T> visitor) {
    visitor.visitLiteral(this);
  }
};

class Unary : Expr {
  public:
  Token op;
  Expr* right;
  Unary( Token op, Expr* right) : op(op), right(right) {}
  template <typename T>
  void accept (ExprAstVisitor<T> visitor) {
    visitor.visitUnary(this);
  }
};
#endif
