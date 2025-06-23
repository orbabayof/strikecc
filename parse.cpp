#include "parse.hpp"
#include "lex.hpp"
#include "ast.hpp"
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string_view>

Parser::Parser(std::list<Token> tokenlist) : _tokenlist(std::move(tokenlist)) {
  // add eof Token
  _tokenlist.emplace_back(eof);
  _currToken = _tokenlist.begin();
}

Program Parser::parse() {
  Program p{};
  while (peek().type != Token::eofToken) {
    Stmt stmt{};
    switch (peek().type) {
    case Token::returnToken: {
      stmt = returnExpr();
      break;
    }
    default: {
      std::cerr << "unimplemented stmt\n";
      std::exit(1);
      break;
    }
    }
    p.emplace_back(stmt);
    consume();
  }
  return p;
}

Expr Parser::expr() {
  switch (peek().type) {
    case Token::integerLiteralToken:
    {
      std::stringstream num {};
      num << peek().str; 
      IntegerLiteral lit {};
      num >> lit.val;

      return { lit };
    }

    default: {
      std::cerr << "unimplemented stmt\n";
      std::exit(1);
    }
  
  }
}

Stmt Parser::returnExpr() {
  consume();
  ReturnStmt ret{.expr = expr()};
  return {ret};
}
const Token &Parser::consume()
{
  std::advance(_currToken, 1);
  return peek();
}
const Token &Parser::peek()
{
  return *_currToken;
}

bool Parser::setTypeOfNext() {
  Token::tokenType typeOfId = peek().type;
  consume();
  if (peek().type != Token::identifierToken) {
    // TODO type casting
    std::cerr << "after a type should come an identifier";
  }

  _idToType[peek().str] = typeOfId;
  return true;
}

Token::tokenType Parser::typeOfCurr() {
  if (!_idToType.contains(peek().str)) {
    std::stringstream err{};
    err << peek().str << " id is not registered";
    throw std::runtime_error(err.str());
  }
  return _idToType[peek().str];
}

bool Parser::checkNext(Token::tokenType type) {
  return std::next(_currToken)->type == type;
}

bool Parser::endOfParsing() { return peek().type == Token::eofToken; }
