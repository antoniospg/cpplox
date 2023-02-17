#ifndef RETURN_H
#define RETURN_H

#include <stdexcept>

#include "Util.h"

class ReturnError : public runtime_error {
public:
  Obj value;
  ReturnError(Obj value) : runtime_error(""), value(value) {}
};

#endif
