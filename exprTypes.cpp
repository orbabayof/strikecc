#include "exprTypes.hpp"
#include <string>

std::string ExprBase::toString() { return "unimplemented toString function"; }

std::string NumberExpr::toString() { return std::to_string(_val); }
