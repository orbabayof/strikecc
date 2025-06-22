#pragma once

#include "lex.hpp"

#include <cstdint>
#include <memory>
#include <stdint.h>
#include <string_view>
#include <vector>

class ExprBase {
public:
  virtual ~ExprBase() = default;

  ExprBase(ExprType type) : _type{type} {}

  virtual std::string toString();

protected:
  ExprType _type{};
};

// for now we only have support for int literals
class NumberExpr : public ExprBase {
public:
  NumberExpr(std::int32_t val) : ExprBase(ExprType::i32_t), _val(val) {}

  std::string toString() override;

private:
  [[maybe_unused]] std::int32_t _val{};
};

class VariableExpr : public ExprBase {
public:
  VariableExpr(std::string_view name, ExprType type)
      : ExprBase(type), _name(name) {}

  std::string toString() override;

private:
  std::string_view _name{};
};

class CallExpr : public ExprBase {
public:
  CallExpr(std::string_view callee, std::vector<std::unique_ptr<ExprBase>> args,
           ExprType type)
      : ExprBase(type), _callee{callee}, _args{std::move(args)} {}

  std::string toString() override;
private:
  std::string_view _callee{};
  std::vector<std::unique_ptr<ExprBase>> _args{};
};

class BinaryExpr : public ExprBase {
public:
  using op_t = Token::tokenType;

  BinaryExpr(std::unique_ptr<ExprBase> left, op_t op,
             std::unique_ptr<ExprBase> right, ExprType type)
      : ExprBase(type), _left(std::move(left)), _right(std::move(right)),
        _op(op) {}

  std::string toString() override;

private:
  std::unique_ptr<ExprBase> _left{};
  std::unique_ptr<ExprBase> _right{};
  [[maybe_unused]] op_t _op{};
};

class TypeExpr : public ExprBase {
public:
  TypeExpr(ExprType type) : ExprBase(type) {}

  std::string toString() override;
};

class PrototypeAst {
public:
  PrototypeAst(std::string_view name,
               std::vector<std::unique_ptr<ExprBase>> args)
      : _name{name}, _args{std::move(args)} {}

private:
  std::string_view _name{};
  std::vector<std::unique_ptr<ExprBase>> _args{};
};

class FunctionAst {
public:
  FunctionAst(std::unique_ptr<PrototypeAst> prototype,
              std::unique_ptr<ExprBase> body)
      : _prototype(std::move(prototype)), _body(std::move(body)) {}

private:
  std::unique_ptr<PrototypeAst> _prototype{};
  std::unique_ptr<ExprBase> _body{};
};
