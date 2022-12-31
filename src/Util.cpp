#include "Util.h"

#include <iostream>

using namespace std;

string to_string(const Obj &val) {
  if (holds_alternative<string>(val))
    return get<string>(val);
  else if (holds_alternative<double>(val))
    return to_string(get<double>(val));
  else if (holds_alternative<bool>(val))
    return to_string(get<bool>(val));
  else
    return "nil";
}

void report(int line, string where, string message) {
  cout << "[line " << line << "] Error" << where << ": " << message << endl;
}
