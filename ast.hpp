#pragma once

#include <functional>
#include <list>
#include <variant>

struct Expr;

using ExprRef = std::reference_wrapper<Expr>;

struct Stmt;

struct IntegerLiteral 
{
  int val;
};

struct Expr 
{
  std::variant<IntegerLiteral> variant;
};

struct ReturnStmt 
{
  Expr expr;
};

using BlockStmt = std::list<Stmt>;

struct Stmt
{
  std::variant<ReturnStmt, BlockStmt> variant;
};

using Program = std::list<Stmt>;



