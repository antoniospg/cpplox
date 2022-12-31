#ifndef LEX_H
#define LEX_H

#include <map>
#include <string>
#include <vector>

#include "Util.h"

typedef std::string string;

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  EOF_TOK
};

class Token {
public:
  int line;
  TokenType type;
  string lexeme;
  Obj literal;

  Token(TokenType type, string lexeme, Obj literal, int line);
  string show_val();
};

class Lexer {
public:
  std::vector<Token> tokens;
  bool err;

  Lexer(string src);
  bool isDigit(char c);
  bool isAlpha(char c);
  bool isAlphaNumeric(char c);
  void getTokens();
  inline char consume();
  inline bool srcEnd();
  inline char lookahead(int offset);
  bool match(char expect);
  void consumeToken();
  void scanString();
  void scanNum();
  void scanIdentifier();
  void addToken(TokenType type);
  void addToken(TokenType type, string literal);
  void error(int line, string message);

private:
  string src;
  uint start, current, line;
  std::map<string, TokenType> keywords;
};

#endif
