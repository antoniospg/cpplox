#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "Lex.h"
#include "gen/Expr.hpp"

using namespace std;

template <typename T>
class Parser {
 public:
  Parser(vector<Token> tokens);

  bool isEnd();

  Token lookahead();

  Token getPrevious();

  Token consume();

  Token consume(TokenType type, string message);

  bool check(TokenType val);

  bool match(vector<TokenType> types);

  Expr<T>* expression();
  Expr<T>* equality();
  Expr<T>* comparison();
  Expr<T>* term();
  Expr<T>* factor();
  Expr<T>* unary();
  Expr<T>* primary();

 private:
  vector<Token> tokens;
  int current;
};

#endif
