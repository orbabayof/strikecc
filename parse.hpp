#include "exprTypes.hpp"
#include <list>
#include <memory>

class Parser {
public:
  Parser(std::list<Token>&& tokenList);

  std::unique_ptr<ExprBase> parseExpr();

  std::unique_ptr<ExprBase> parseNumberExpr();
  std::unique_ptr<ExprBase> parseParenExpr();
  std::unique_ptr<ExprBase> parseIdentifierExpr();
  std::unique_ptr<ExprBase> parseBinaryExpr();
  std::unique_ptr<ExprBase> parseTypeExpr();

  const Token &nextToken();
  const Token &currToken();

  void setTypeOfNext();
  ExprType typeOfCurr();

  bool isNextToken(Token::tokenType type);
  bool endOfParsing();

  friend int main();

private:
  std::list<Token> _tokenlist{};
  std::list<Token>::iterator _currToken{};

  std::unordered_map<std::string_view, ExprType> _idToType{};

  static inline Token eof { Token::eofToken, "eof" };

  std::unique_ptr<ExprBase> parsePrimary();
};
