#pragma once
#include <list>
#include "lex.hpp"
#include "ast.hpp"

class Parser {
public:
  Parser(std::list<Token> tokenList);

  Program parse();
  Expr expr();

  Stmt returnExpr();
  const Token &consume();
  const Token &peek();

  bool setTypeOfNext();
  Token::tokenType typeOfCurr();

  bool checkNext(Token::tokenType type);
  bool endOfParsing();

  friend int main();

  [[nodiscard]] std::string codeGen();

private:
  std::list<Token> _tokenlist{};
  std::list<Token>::iterator _currToken{};

  std::unordered_map<std::string_view, Token::tokenType> _idToType{};

  static inline Token eof{Token::eofToken, "eof"};
};
