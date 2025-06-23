#pragma once
#include <cassert>
#include <list>
#include <string_view>
#include <unordered_map>

enum class ExprType {
  i8_t,
  i16_t,
  i32_t,

  float_t,
  double_t,

  void_t,
  struct_t,
};

std::string_view toString(ExprType type);

struct Token {
  // remember to add reserved ones to table
  enum tokenType {
    closeBraceToken,
    openBraceToken,
    closeParentToken,
    openParentToken,

    semicolonToken,
    commaToken,

    intToken,
    doubleToken,
    floatToken,
    voidToken,
    structToken,

    returnToken,

    integerLiteralToken,
    identifierToken,

    plusToken,
    minusToken,
    devToken,
    multToken,
    assignToken,

    eofToken,
  };

  static inline std::unordered_map<std::string_view, tokenType>
      reservedKeywordTokenTypeMapper{
          {"int", intToken},       {"double", doubleToken},
          {"float", floatToken},   {"void", voidToken},

          {"return", returnToken},

          {"{", openBraceToken},   {"}", closeBraceToken},
          {"(", openParentToken},  {")", closeParentToken},
          {";", semicolonToken},   {",", commaToken},

          {"-", minusToken},       {"+", plusToken},
          {"/", devToken},         {"*", multToken},
      };

  Token(std::string_view token);
  Token(tokenType typeOfToken, std::string_view tokenStr);

  static std::list<Token> lexLine(std::string_view line);

  tokenType type;
  std::string_view str;

  static bool isBrace(char c);
  static std::string_view strTokenFromLine(std::string_view line);
  static std::string_view toString(tokenType type);

  static tokenType toTokenType(std::string_view token);
};

