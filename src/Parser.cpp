#include "Parser.h"

#include <iostream>
#include <vector>

#include "Lex.h"
#include "Util.h"

using namespace std;

// Aux functions
template <typename T>
Parser<T>::Parser(vector<Token> tokens)
    : err(false), tokens(tokens), current(0) {}

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

  throw error(lookahead(), message);
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

template <typename T>
ParserError Parser<T>::error(Token token, string message) {
  err = true;
  if (token.type == EOF_TOK)
    report(token.line, " at end", message);
  else
    report(token.line, " at '" + token.lexeme + "'", message);
  return ParserError();
}

template <typename T>
void Parser<T>::synchronize() {
  consume();

  while (!isEnd()) {
    if (getPrevious().type == SEMICOLON) return;

    switch (lookahead().type) {
      case CLASS:
      case FUN:
      case VAR:
      case FOR:
      case IF:
      case WHILE:
      case PRINT:
      case RETURN:
        return;
    }

    consume();
  }
}

// Build AST
template <typename T>
vector<Stmt<T> *> Parser<T>::parse() {
  try {
    vector<Stmt<T> *> statements;
    while (!isEnd()) {
      auto stmt = declaration();
      if (stmt != nullptr) statements.push_back(stmt);
    }

    return statements;
  } catch (const ParserError &err) {
    return vector<Stmt<T> *>();
  }
}

template <typename T>
Expr<T> *Parser<T>::expression() {
  return assignment();
}

template <typename T>
Expr<T> *Parser<T>::assignment() {
  Expr<T> *expr = equality();

  if (match({EQUAL})) {
    Token equals = getPrevious();
    Expr<T> *value = assignment();

    auto *variable = dynamic_cast<Variable<T> *>(expr);
    if (variable) {
      Token name = variable->name;
      return new Assign<T>(name, value);
    }

    error(equals, "Invalid assignment target.");
  }

  return expr;
}

template <typename T>
Expr<T> *Parser<T>::equality() {
  Expr<T> *expr = comparison();

  while (match({BANG, BANG_EQUAL})) {
    auto op = getPrevious();
    Expr<T> *right = comparison();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T> *Parser<T>::comparison() {
  Expr<T> *expr = term();

  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    auto op = getPrevious();
    Expr<T> *right = term();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T> *Parser<T>::term() {
  Expr<T> *expr = factor();

  while (match({MINUS, PLUS})) {
    auto op = getPrevious();
    Expr<T> *right = factor();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T> *Parser<T>::factor() {
  Expr<T> *expr = unary();

  while (match({SLASH, STAR})) {
    auto op = getPrevious();
    Expr<T> *right = unary();

    expr = new Binary<T>(expr, op, right);
  }
  return expr;
}

template <typename T>
Expr<T> *Parser<T>::unary() {
  if (match({BANG, MINUS})) {
    auto op = getPrevious();
    Expr<T> *expr = unary();

    expr = new Unary<T>(op, expr);
    return expr;
  }

  return primary();
}

template <typename T>
Expr<T> *Parser<T>::primary() {
  if (match({FALSE}))
    return new Literal<T>("false");
  else if (match({TRUE}))
    return new Literal<T>("true");
  else if (match({NIL}))
    return new Literal<T>("nil");
  else if (match({NUMBER, STRING}))
    return new Literal<T>(getPrevious().literal);
  else if (match({IDENTIFIER}))
    return new Variable<T>(getPrevious());
  else if (match({LEFT_PAREN})) {
    Expr<T> *expr = expression();

    consume(RIGHT_PAREN, "Expected ')' after expression");
    return new Grouping<T>(expr);
  } else {
    throw error(lookahead(), "Expected expression");
    err = true;
    return nullptr;
  }
}

template <typename T>
Stmt<T> *Parser<T>::declaration() {
  try {
    if (match({VAR})) return varDeclaration();
    return statement();
  } catch (ParserError error) {
    // sync();
    return nullptr;
  }
}

template <typename T>
Stmt<T> *Parser<T>::varDeclaration() {
  Token name = consume(IDENTIFIER, "Expect variable name.");

  Expr<T> *initializer = nullptr;
  if (match({EQUAL})) {
    initializer = expression();
  }

  consume(SEMICOLON, "Expect ';' after variable declaration");
  return new Var<T>(name, initializer);
}

template <typename T>
Stmt<T> *Parser<T>::statement() {
  if (match({PRINT})) return printStatement();

  return expressionStatement();
}

template <typename T>
Stmt<T> *Parser<T>::printStatement() {
  Expr<T> *value = expression();
  consume(SEMICOLON, "Expected ';' after expression");

  return new Print<T>(value);
}

template <typename T>
Stmt<T> *Parser<T>::expressionStatement() {
  Expr<T> *value = expression();
  consume(SEMICOLON, "Expected ';' after value");

  return new Expression<T>(value);
}

template class Parser<string>;
template class Parser<Obj>;
