#include "AstPrinter.h"

#include <iostream>
#include <variant>
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
  if (holds_alternative<monostate>(expr->value))
    return "nil";
  else
    return to_string(expr->value);
}

string AstPrinter::visitExprUnary(Unary<string>* expr) {
  return parenthesize(expr->op.lexeme, {expr->right});
}
