#include "parse.hpp"
#include "exprTypes.hpp"
#include "lex.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string_view>

Parser::Parser(std::list<Token> &&tokenlist)
    : _tokenlist(std::move(tokenlist)) {
  // add eof Token
  _tokenlist.emplace_back(eof);
  _currToken = _tokenlist.begin();
}

std::unique_ptr<ExprBase> Parser::parseParenExpr() {

  // eat Token::openParent
  nextToken();
  auto v = parseExpr();
  if (!v) {
    return nullptr;
  }
  if (currToken().type == Token::closeParentToken) {
    throw std::runtime_error("expected )");
  }

  // eat Token::closeParent
  nextToken();

  return v;
}

std::unique_ptr<ExprBase> Parser::parseIdentifierExpr() {
  std::string_view id = currToken().str;

  ExprType idType{typeOfCurr()};
  //eats id
  if (nextToken().type != Token::openParentToken) {
    // simple variable
    return std::make_unique<VariableExpr>(id, idType);
  }

  std::vector<std::unique_ptr<ExprBase>> args{};
  while (currToken().type != Token::closeParentToken) {
    if (std::unique_ptr<ExprBase> currArg{parseExpr()}; currArg != nullptr) {
      args.emplace_back(std::move(currArg));

      if (currToken().type == Token::openParentToken) {
        break;
      }
      if (currToken().type != Token::commaToken) {
        throw std::runtime_error(
            "Parser::parseIdentifierExpr(): Expected ) or , in argument list");
      }

      nextToken();
    } else {
      std::cerr
          << "Parser::parseIdentifierExpr(): call expr syntax is not correct";
      return nullptr;
    }

    // eat )
    nextToken();

    return std::make_unique<CallExpr>(id, std::move(args), idType);
  }

  return nullptr;
}


std::unique_ptr<ExprBase> Parser::parseBinaryExpr() { return {


}; }

std::unique_ptr<ExprBase> Parser::parsePrimary()
{
  using enum Token::tokenType;
  switch (currToken().type) 
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
      nextToken();
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


const Token &Parser::currToken() { return *_currToken; }
const Token &Parser::nextToken() {
  std::advance(_currToken, 1);
  return currToken();
}

void Parser::setTypeOfNext() {
  auto switchType = [this]() {
    // TODO add more types
    switch (currToken().type) {
    case Token::intToken:
      return ExprType::i32_t;
    case Token::doubleToken:
      return ExprType::double_t;
    case Token::floatToken:
      return ExprType::float_t;
    case Token::voidToken:
      return ExprType::void_t;
    case Token::structToken:
      return ExprType::struct_t;

    default: {
      std::ostringstream out{};
      out << "cannot get type of " << _currToken->type;
      throw std::runtime_error(out.str());
    }
    }
  };

  ExprType Idtype{switchType()};
  nextToken();
  if (currToken().type != Token::identifierToken) {
    // TODO type casting
    throw std::runtime_error("after a type should come an identifier");
  }

  _idToType[currToken().str] = Idtype;
}

ExprType Parser::typeOfCurr() {
  if (!_idToType.contains(currToken().str))
  {
    std::stringstream err {};
    err << currToken().str << " id is not registered";
    throw std::runtime_error(err.str());
  }
  return _idToType[currToken().str];
}

bool Parser::isNextToken(Token::tokenType type) {
  return std::next(_currToken)->type == type;
}

bool Parser::endOfParsing() { return currToken().type == Token::eofToken; }

std::unique_ptr<ExprBase> Parser::parseTypeExpr() {
  // eats type
  setTypeOfNext();
  // now we are at the identifier token
  return std::make_unique<TypeExpr>(typeOfCurr());
}
std::unique_ptr<ExprBase> Parser::parseNumberExpr() {
  std::string_view numAsStr{currToken().str};
  std::stringstream stream{};
  stream << numAsStr;

  int val;
  stream >> val;

  auto numExpr{std::make_unique<NumberExpr>(val)};
  nextToken();
  return numExpr;
}
