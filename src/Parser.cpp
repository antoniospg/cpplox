#include "Parser.h"

#include <iostream>
#include <vector>

#include "Lex.h"

using namespace std;

// Aux functions
template <typename T>
Parser<T>::Parser(vector<Token> tokens) : tokens(tokens), current(0) {}

template <typename T>
bool Parser<T>::isEnd() {
  return lookahead().type == EOF_TOK;
}

template <typename T>
Token Parser<T>::lookahead() {
  return tokens[current];
}

template <typename T>
Token Parser<T>::getPrevious() {
  return tokens[current - 1];
}

template <typename T>
Token Parser<T>::consume() {
  if (!isEnd()) current++;
  return getPrevious();
}

template <typename T>
Token Parser<T>::consume(TokenType type, string message) {
  if (check(type)) return consume();

  cout << message << endl;
  exit(1);
}

template <typename T>
bool Parser<T>::check(TokenType val) {
  if (isEnd()) return false;

  return lookahead().type == val;
}

template <typename T>
bool Parser<T>::match(vector<TokenType> types) {
  for (auto type : types) {
    if (check(type)) {
      consume();
      return true;
    }
  }
  return false;
}

// Build AST
template <typename T>
Expr<T>* Parser<T>::expression() {
  return equality();
}

template <typename T>
Expr<T>* Parser<T>::equality() {
  Expr<T>* expr = comparison();

  while (match({BANG, BANG_EQUAL})) {
    auto op = getPrevious();
    Expr<T>* right = comparison();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T>* Parser<T>::comparison() {
  Expr<T>* expr = term();

  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    auto op = getPrevious();
    Expr<T>* right = term();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T>* Parser<T>::term() {
  Expr<T>* expr = factor();

  while (match({MINUS, PLUS})) {
    auto op = getPrevious();
    Expr<T>* right = factor();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T>* Parser<T>::factor() {
  Expr<T>* expr = unary();

  while (match({SLASH, STAR})) {
    auto op = getPrevious();
    Expr<T>* right = unary();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T>* Parser<T>::unary() {
  if (match({BANG, MINUS})) {
    auto op = getPrevious();
    Expr<T>* expr = unary();

    expr = new Unary<T>(op, expr);
    return expr;
  }

  return primary();
}

template <typename T>
Expr<T>* Parser<T>::primary() {
  if (match({FALSE}))
    return new Literal<T>("false");
  else if (match({TRUE}))
    return new Literal<T>("true");
  else if (match({NIL}))
    return new Literal<T>("nil");
  else if (match({NUMBER, STRING}))
    return new Literal<T>(getPrevious().literal);
  else if (match({LEFT_PAREN})) {
    Expr<T>* expr = expression();

    consume(RIGHT_PAREN, "Expected ')' after expression");
    return new Grouping<T>(expr);
  } else
    return nullptr;
}

template class Parser<string>;
