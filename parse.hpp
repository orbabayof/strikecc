#include "exprTypes.hpp"
#include <list>
#include <memory>

class Parser {
public:
  Parser(std::list<Token> tokenList);

  std::unique_ptr<ExprBase> parseExpr();

  std::unique_ptr<ExprBase> parseNumberExpr();
  std::unique_ptr<ExprBase> parseParenExpr();
  std::unique_ptr<ExprBase> parseIdentifierExpr();
  std::unique_ptr<ExprBase> parseBinaryExpr();
  std::unique_ptr<ExprBase> parseTypeExpr();

private:
  Lexer _lexer;

  std::unique_ptr<ExprBase> parsePrimary();
};
