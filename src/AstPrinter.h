#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include <string>
#include <vector>

#include "gen/Expr.hpp"

using namespace std;

class AstPrinter : public ExprAstVisitor<string> {
public:
  string print(Expr<string> *expr);
  string parenthesize(string name, vector<Expr<string> *> exprs);
  string visitExprBinary(Binary<string> *expr) override;
  string visitExprGrouping(Grouping<string> *expr) override;
  string visitExprLiteral(Literal<string> *expr) override;
  string visitExprUnary(Unary<string> *expr) override;
};

#endif
