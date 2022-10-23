#ifndef LEX_H
#define LEX_H

#include <string>
#include <vector>

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
  string literal;

  Token(TokenType type, string lexeme, string literal);

  string show_val();
};

class Lexer {
 public:
  static bool err;

  Lexer(string src);

  void getTokens();

  char consume();

  void consumeToken();

  void addToken(TokenType type);

  void addToken(TokenType type, string literal);

  void error(int line, string message);

  void report(int line, string where, string message);

 private:
  string src;
  uint start, current, line;
  std::vector<Token> tokens;
};

#endif
