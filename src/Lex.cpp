#include "Lex.h"

#include <iostream>

typedef std::string string;

std::map<TokenType, string> tok_to_string;

// ----------------------------------------
// Token function definitions
Token::Token(TokenType type, string lexeme, string literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

string Token::show_val() {
  return string("type: '" + tok_to_string[type] + "' lexeme: '" + lexeme +
                "' literal: '" + literal + "'");
}

// ----------------------------------------
// Lexer function definitions
bool Lexer::err = false;

Lexer::Lexer(string src) : src(src), start(0), current(0), line(1) {
  // Keywords for lox
  keywords["and"] = AND;
  keywords["class"] = CLASS;
  keywords["else"] = ELSE;
  keywords["false"] = FALSE;
  keywords["for"] = FOR;
  keywords["fun"] = FUN;
  keywords["if"] = IF;
  keywords["nil"] = NIL;
  keywords["or"] = OR;
  keywords["print"] = PRINT;
  keywords["return"] = RETURN;
  keywords["super"] = SUPER;
  keywords["this"] = THIS;
  keywords["true"] = TRUE;
  keywords["var"] = VAR;
  keywords["while"] = WHILE;

  // Enum To string
  tok_to_string[LEFT_PAREN] = "LEFT_PAREN";
  tok_to_string[RIGHT_PAREN] = "RIGHT_PAREN";
  tok_to_string[LEFT_BRACE] = "LEFT_BRACE";
  tok_to_string[RIGHT_BRACE] = "RIGHT_BRACE";
  tok_to_string[COMMA] = "COMMA";
  tok_to_string[DOT] = "DOT";
  tok_to_string[MINUS] = "MINUS";
  tok_to_string[PLUS] = "PLUS";
  tok_to_string[SEMICOLON] = "SEMICOLON";
  tok_to_string[SLASH] = "SLASH";
  tok_to_string[STAR] = "STAR";
  tok_to_string[BANG] = "BANG";
  tok_to_string[BANG_EQUAL] = "BANG_EQUAL";
  tok_to_string[EQUAL] = "EQUAL";
  tok_to_string[EQUAL_EQUAL] = "EQUAL_EQUAL";
  tok_to_string[GREATER] = "GREATER";
  tok_to_string[GREATER_EQUAL] = "GREATER_EQUAL";
  tok_to_string[LESS] = "LESS";
  tok_to_string[LESS_EQUAL] = "LESS_EQUAL";
  tok_to_string[IDENTIFIER] = "IDENTIFIER";
  tok_to_string[STRING] = "STRING";
  tok_to_string[NUMBER] = "NUMBER";
  tok_to_string[AND] = "AND";
  tok_to_string[CLASS] = "CLASS";
  tok_to_string[ELSE] = "ELSE";
  tok_to_string[FALSE] = "FALSE";
  tok_to_string[FUN] = "FUN";
  tok_to_string[FOR] = "FOR";
  tok_to_string[IF] = "IF";
  tok_to_string[NIL] = "NIL";
  tok_to_string[OR] = "OR";
  tok_to_string[PRINT] = "PRINT";
  tok_to_string[RETURN] = "RETURN";
  tok_to_string[SUPER] = "SUPER";
  tok_to_string[THIS] = "THIS";
  tok_to_string[TRUE] = "TRUE";
  tok_to_string[VAR] = "VAR";
  tok_to_string[WHILE] = "WHILE";
  tok_to_string[EOF_TOK] = "EOF_TOK";
}

void Lexer::error(int line, string message) { report(line, "", message); }

void Lexer::report(int line, string where, string message) {
  std::cout << "[line " << line << "] Error" << where << ": " << message
            << std::endl;
  err = true;
}

bool Lexer::isDigit(char c) { return c <= '9' && c >= '0'; }

bool Lexer::isAlpha(char c) {
  return (c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c == '_');
}

bool Lexer::isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

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
  tokens.push_back(
      Token(type, src.substr(start, current - start), literal, line));
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

void Lexer::scanIdentifier() {
  while (isAlphaNumeric(lookahead(0))) consume();
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

    // Special cases
    default:
      // Check if it's a digit
      if (isDigit(c)) {
        scanNum();
        addToken(NUMBER, src.substr(start, current - start));
        // Check if it's a identifier
      } else if (isAlpha(c)) {
        scanIdentifier();

        // Check if it's a keyword
        string identifierText = src.substr(start, current - start);
        TokenType tokType = (keywords.find(identifierText) == keywords.end())
                                ? IDENTIFIER
                                : keywords[identifierText];
        addToken(tokType, identifierText);
        // Error
      } else {
        error(line, string("Unexpected character: ") + c);
        err = true;
      }
      break;
  }
}

void Lexer::getTokens() {
  std::cout << "Start lexing..." << std::endl;
  while (!srcEnd()) {
    consumeToken();
    start = current;
  }

  addToken(EOF_TOK);

  for (auto& t : tokens) {
    std::cout << t.show_val() << std::endl;
  }
}
