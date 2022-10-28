#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Lex.h"
#include "gen/Expr.hpp"

typedef std::string string;

static string readAllBytes(char const* filename) {
  std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
  if (!ifs.good()) exit(66);

  std::ifstream::pos_type pos = ifs.tellg();

  std::vector<char> val(pos);

  ifs.seekg(0, std::ios::beg);
  ifs.read(&val[0], pos);

  return string(val.begin(), val.end());
}

bool run(string src) {
  Lexer* scan = new Lexer(src);
  scan->getTokens();
  bool err =  scan->err;
  scan->err = false;

  return err;
}

int repl() {
  for (;;) {
    string line;
    std::cout << "cpplox>";
    std::getline(std::cin, line);

    if (std::cin.eof()) {
      std::cout << std::endl;
      break;
    }

    run(line);
    std::cout << "endl line" << std::endl;
  }

  return 0;
}

int runFile(const string& path) {
  string src = readAllBytes(path.data());
  bool err = run(src);

  if (err) exit(65);

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    std::cout << "Usage: cpplox [script]" << std::endl;
    exit(64);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    repl();
  }

  exit(0);
}
