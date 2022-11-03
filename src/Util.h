#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <variant>

using namespace std;

// Varinant to encode possible object values
typedef variant<monostate, string, double, bool> Obj;

string to_string(const Obj& val);

#endif
