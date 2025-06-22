#include "exprTypes.hpp"
#include <sstream>
#include <string>

std::string ExprBase::toString() { return "unimplemented toString function"; }

std::string NumberExpr::toString() { 
  std::ostringstream out {};
  out << "NumberExpr{ " << ::toString(_type) << " : " << _val;
  return out.str();
}
std::string VariableExpr::toString() { 
  std::ostringstream out {};
  out << "VariableExpr{ " << ::toString(_type) << " : " << _name;
  return out.str();
}
std::string CallExpr::toString() { 
  return "unimlemented callExpr::toString()";
}
std::string BinaryExpr::toString() { return "unimplemented toString function"; }
std::string TypeExpr::toString() { 
  std::ostringstream out {};
  out << "TypeExpr{ " << ::toString(_type);
  return out.str();
}
