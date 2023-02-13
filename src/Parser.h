#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <stdexcept>
#include <string>
#include <vector>

#include "Lex.h"
#include "gen/Expr.hpp"
#include "gen/Stmt.hpp"

using namespace std;

class ParserError : public runtime_error {
 public:
  ParserError() : runtime_error("") {}
};

template <typename T>
class Parser {
 public:
  bool err;

  Parser(vector<Token> tokens);
  bool isEnd();
  Token lookahead();
  Token getPrevious();
  Token consume();
  Token consume(TokenType type, string message);
  bool check(TokenType val);
  bool match(vector<TokenType> types);
  ParserError error(Token token, string message);
  void synchronize();
  vector<Stmt<T> *> parse();
  Expr<T> *assignment();
  Expr<T> *expression();
  Expr<T> *orOp();
  Expr<T> *andOp();
  Expr<T> *equality();
  Expr<T> *comparison();
  Expr<T> *term();
  Expr<T> *factor();
  Expr<T> *unary();
  Expr<T> *call();
  Expr<T> *finishCall(Expr<T> *callee);
  Expr<T> *primary();
  Stmt<T> *declaration();
  Stmt<T> *function(string kind);
  Stmt<T> *varDeclaration();
  Stmt<T> *whileStatement();
  Stmt<T> *statement();
  Stmt<T> *forStatement();
  Stmt<T> *ifStatement();
  Stmt<T> *printStatement();
  list<Stmt<T> *> block();
  Stmt<T> *expressionStatement();

 private:
  vector<Token> tokens;
  int current;
};

#endif
