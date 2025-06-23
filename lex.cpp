#include "lex.hpp"

#include <cassert>
#include <cctype>
#include <cstddef>
#include <list>
#include <string_view>

std::string_view toString(ExprType type) {
  using enum ExprType;
  switch (type) {
  case i32_t:
    return "int";
  case i16_t:
    return "short";
  case i8_t:
    return "char";
  case float_t:
    return "float";
  case double_t:
    return "double";
  case struct_t:
    return "struct";
  case void_t:
    return "void";
  }
}
static constexpr void clearSpacePrefix(std::string_view &str) {
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
    if (length > 1 && line[length] == ';') {
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

  case eofToken:
    return "eof";
  }

  return "";
}

Token::Token(tokenType typeOfToken, std::string_view tokenStr)
    : type(typeOfToken), str(tokenStr) {}

Token::Token(std::string_view tokenStr)
    : Token(toTokenType(tokenStr), tokenStr) {}

