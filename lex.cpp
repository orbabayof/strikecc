#include "lex.hpp"

#include <cassert>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string_view>

std::string_view toString(ExprType type) {
  using enum ExprType;
  switch (type) {
    case i32_t: return "int";
    case i16_t: return "short";
    case i8_t: return "char";
    case float_t: return "float";
    case double_t: return "double";
    case struct_t: return "struct";
    case void_t: return "void";
  }
}
static inline void clearSpacePrefix(std::string_view &str) {
  while (str.length() > 0 && std::isspace(str.front()))
    str.remove_prefix(1);
}

Token::tokenType Token::toTokenType(std::string_view token) {
  if (reservedKeywordTokenTypeMapper.contains(token)) {
    return reservedKeywordTokenTypeMapper[token];
  }

  if (std::isdigit(token.front())) {
    for (char e : token) {
      assert(std::isdigit(e) && !std::isspace(e));
    }
    return integerLiteralToken;
  }

  // TODO check for identifier conventions
  return identifierToken;
}

bool Token::isBrace(char c) {
  switch (c) {
  case '[':
  case ']':

  case ')':
  case '(':

  case '{':
  case '}':
    return true;
  }

  return false;
}

std::string_view Token::strTokenFromLine(std::string_view line) {
  clearSpacePrefix(line);

  std::size_t length{0};
  while (length < line.length() && !std::isspace(line[length])) {
    if (isBrace(line[length]) && length != 0) {
      break;
    }
    ++length;
  }

  return line.substr(0, length);
}

std::list<Token> Token::lexLine(std::string_view line) {
  std::list<Token> list{};
  std::string_view currTokenStr{};
  clearSpacePrefix(line);
  while (!(currTokenStr = strTokenFromLine(line)).empty()) {
    line.remove_prefix(currTokenStr.length());
    clearSpacePrefix(line);
    list.emplace_back(Token{currTokenStr});
  }

  return list;
}

std::string_view Token::toString(tokenType type) {
  switch (type) {
  case closeBraceToken:
    return "closeBrace";
  case openBraceToken:
    return "openBrace";
  case closeParentToken:
    return "closeParent";
  case openParentToken:
    return "openParent";

  case semicolonToken:
    return "semicolon";
  case commaToken:
    return "comma";

  case intToken:
    return "int";
  case floatToken:
    return "float";
  case doubleToken:
    return "double";
  case voidToken:
    return "void";
  case structToken:
    return "struct";

  case returnToken:
    return "return";
  case integerLiteralToken:
    return "integerLiteral";
  case identifierToken:
    return "identifier";
  }
  return "";
}

Token::Token(std::string_view tokenStr) : str(tokenStr) {
  type = toTokenType(str);
}

Lexer::Lexer(std::list<Token> &&tokenlist)
    : _tokenlist(std::move(tokenlist)), _currToken(_tokenlist.begin()) {
  assert(!_tokenlist.empty());
}
const Token &Lexer::currToken() {
  if (_currToken != _tokenlist.end())
    return *_currToken;

  throw std::runtime_error("you got to the end of the list");
}
const Token &Lexer::nextToken() {
  std::advance(_currToken, 1);
  return currToken();
}

void Lexer::setTypeOfNext() {
  auto switchType = [this]() {
    if (_currToken == _tokenlist.begin())
      throw std::runtime_error("LexView: could not get type of curr");

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

ExprType Lexer::typeOfCurr() {
  if(!_idToType.contains(currToken().str))
    throw std::runtime_error((std::ostringstream{} << currToken().str << " id is not registered").str());

  return _idToType[currToken().str];
}
