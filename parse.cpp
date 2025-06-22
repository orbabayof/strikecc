#include "parse.hpp"
#include "exprTypes.hpp"
#include "lex.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>


Parser::Parser(std::list<Token>&& tokenList): _lexer(std::move(tokenList)) {}
std::unique_ptr<ExprBase> Parser::parseTypeExpr() {
  // eats type
  _lexer.setTypeOfNext();
  // now we are at the identifier token
  return std::make_unique<TypeExpr>(_lexer.typeOfCurr());
}
std::unique_ptr<ExprBase> Parser::parseNumberExpr() {
  std::string_view numAsStr{_lexer.currToken().str};
  std::stringstream stream{};
  stream << numAsStr;

  int val;
  stream >> val;

  auto numExpr{std::make_unique<NumberExpr>(val)};
  _lexer.nextToken();
  return numExpr;
}

std::unique_ptr<ExprBase> Parser::parseParenExpr() {

  // eat Token::openParent
  _lexer.nextToken();
  auto v = parseExpr();
  if (!v) {
    return nullptr;
  }
  if (_lexer.currToken().type == Token::closeParentToken) {
    throw std::runtime_error("expected )");
  }

  // eat Token::closeParent
  _lexer.nextToken();

  return v;
}

std::unique_ptr<ExprBase> Parser::parseIdentifierExpr() {
  std::string_view id = _lexer.currToken().str;

  ExprType idType{_lexer.typeOfCurr()};
  //eats id
  if (_lexer.nextToken().type != Token::openParentToken) {
    // simple variable
    return std::make_unique<VariableExpr>(id, idType);
  }

  std::vector<std::unique_ptr<ExprBase>> args{};
  while (_lexer.currToken().type != Token::closeParentToken) {
    if (std::unique_ptr<ExprBase> currArg{parseExpr()}; currArg != nullptr) {
      args.emplace_back(std::move(currArg));

      if (_lexer.currToken().type == Token::openParentToken) {
        break;
      }
      if (_lexer.currToken().type != Token::commaToken) {
        throw std::runtime_error(
            "Parser::parseIdentifierExpr(): Expected ) or , in argument list");
      }

      _lexer.nextToken();
    } else {
      std::cerr
          << "Parser::parseIdentifierExpr(): call expr syntax is not correct";
      return nullptr;
    }

    // eat )
    _lexer.nextToken();

    return std::make_unique<CallExpr>(id, std::move(args), idType);
  }

  return nullptr;
}


std::unique_ptr<ExprBase> Parser::parseBinaryExpr() { return {


}; }

std::unique_ptr<ExprBase> Parser::parsePrimary()
{
  using enum Token::tokenType;
  switch (_lexer.currToken().type) 
  {
    case identifierToken: return parseIdentifierExpr();
    case integerLiteralToken: return parseNumberExpr();
    case openParentToken: return parseParenExpr();
    case intToken: 
    case doubleToken: 
    case floatToken:
    case voidToken:
    case structToken:
        return parseTypeExpr();

    case semicolonToken:
    {
      _lexer.nextToken();
      return nullptr;
    }
    default:
    {
      throw std::runtime_error("unexpected token when parsing");
    }
  }
}

std::unique_ptr<ExprBase> Parser::parseExpr()
{
  return parsePrimary();
}


