#include "Lex.h"

#include <iostream>

typedef std::string string;

// ----------------------------------------
// Token function definitions
Token::Token(TokenType type, string lexeme, string literal)
    : type(type), lexeme(lexeme), literal(literal) {}

string Token::show_val() { return lexeme + " " + literal; }

// ----------------------------------------
// Lexer function definitions
bool Lexer::err = false;

Lexer::Lexer(string src) : src(src), start(0), current(0), line(1) {}

void Lexer::error(int line, string message) { report(line, "", message); }

void Lexer::report(int line, string where, string message) {
  std::cout << "[line " << line << "] Error" << where << ": " << message
            << std::endl;
  err = true;
}

bool Lexer::isDigit(char c) { return c <= '9' && c >= '0'; }

inline char Lexer::consume() { return src[current++]; }

inline bool Lexer::srcEnd() { return current >= src.size(); }

inline char Lexer::lookahead(int offset) {
  if (current + offset >= src.size()) return '\0';
  return src[current + offset];
}

bool Lexer::match(char expect) {
  if (srcEnd()) return false;

  if (src[current] == expect) {
    current++;
    return true;
  }

  return false;
}

void Lexer::addToken(TokenType type) { addToken(type, ""); }

void Lexer::addToken(TokenType type, string literal) {
  tokens.push_back(Token(type, src.substr(start, current - start), literal));
}

void Lexer::scanString() {
  // Consume chars until reach last '"'
  while (lookahead(0) != '"' && !srcEnd()) {
    if (lookahead(0) == '\n') line++;

    consume();
  }

  // If reach end, fail
  if (srcEnd()) {
    error(line, "Expect end of string");
    exit(1);
  }

  // Consume last '"'
  consume();
}

void Lexer::scanNum() {
  while (isDigit(lookahead(0))) consume();

  if (lookahead(0) == '.' && isDigit(lookahead(1))) {
    // Consume '.'
    consume();

    while (isDigit(lookahead(0))) consume();
  }
}

void Lexer::consumeToken() {
  char c = consume();

  switch (c) {
    // Single char tokens
    case '(':
      addToken(LEFT_PAREN);
      break;
    case ')':
      addToken(RIGHT_PAREN);
      break;
    case '{':
      addToken(LEFT_BRACE);
      break;
    case '}':
      addToken(RIGHT_BRACE);
      break;
    case ',':
      addToken(COMMA);
      break;
    case '.':
      addToken(DOT);
      break;
    case '-':
      addToken(MINUS);
      break;
    case '+':
      addToken(PLUS);
      break;
    case ';':
      addToken(SEMICOLON);
      break;
    case '*':
      addToken(STAR);
      break;
    case ' ':
      break;
    case '\n':
      line++;
      break;
    default:
      // Check if it's a digit
      if (isDigit(c)) {
        scanNum();
        addToken(NUMBER, src.substr(start, current - start));
      } else {
        error(line, string("Unexpected character: ") + c);
        err = true;
      }
      break;

    // 2 char tokens
    case '!':
      addToken(match('=') ? BANG_EQUAL : BANG);
      break;
    case '=':
      addToken(match('=') ? EQUAL_EQUAL : EQUAL);
      break;
    case '<':
      addToken(match('=') ? LESS_EQUAL : LESS);
      break;
    case '>':
      addToken(match('=') ? GREATER_EQUAL : GREATER);
      break;

    // Slashes
    case '/':
      if (match('/'))
        while (lookahead(0) != '\n' && !srcEnd()) consume();
      else
        addToken(SLASH);
      break;

    // String literal
    case '"':
      scanString();
      addToken(STRING, src.substr(start + 1, current - start - 2));
  }
}

void Lexer::getTokens() {
  std::cout << "Start lexing..." << std::endl;
  while (!srcEnd()) {
    start = current;
    consumeToken();
  }

  addToken(EOF_TOK);

  for (auto& t : tokens) {
    std::cout << t.show_val() << std::endl;
  }
}
