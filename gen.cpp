#include "gen.hpp"
#include "ast.hpp"
#include <format>
#include <sstream>
#include <string_view>
#include <variant>

static std::string genMove(std::string_view reg, const IntegerLiteral& lit)
{
  return std::format("mov {}, {}\n", reg, lit.val);
}

std::string Generator::generate()
{
  std::ostringstream out {};
  struct Visitor
  {
    std::string operator() (const ReturnStmt& ret) const 
    {
      std::ostringstream gen {};
      gen << genMove("rdi", std::get<IntegerLiteral>(ret.expr.variant));
      gen << genMove("rax", {60});

      return gen.str();
    }

    std::string operator() ([[maybe_unused]]BlockStmt block) const 
    {
      return "";
    }

  };

  for(Stmt& stmt : _program)
  {
    std::variant var = stmt.variant;
    out << std::visit(Visitor{}, var);
  }

  return out.str();
}
