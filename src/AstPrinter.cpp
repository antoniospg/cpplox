#include "AstPrinter.h"

#include <iostream>
#include <vector>

#include "Lex.h"

using namespace std;

string AstPrinter::print(Expr<string>* expr) { return expr->accept(this); }

string AstPrinter::parenthesize(string name, vector<Expr<string>*> exprs) {
  string builder = "";
  builder += "(" + name;
  for (auto expr : exprs) builder += " " + expr->accept(this);
  builder += ")";

  return builder;
}

string AstPrinter::visitExprBinary(Binary<string>* expr) {
  return parenthesize(expr->op.lexeme, {expr->left, expr->right});
}

string AstPrinter::visitExprGrouping(Grouping<string>* expr) {
  return parenthesize("group", {expr->grouping});
}

string AstPrinter::visitExprLiteral(Literal<string>* expr) {
  if (expr->value == "")
    return "nil";
  else
    return expr->value;
}

string AstPrinter::visitExprUnary(Unary<string>* expr) {
  return parenthesize(expr->op.lexeme, {expr->right});
}
