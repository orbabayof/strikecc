#include <iostream>
#include <string_view>

#include "lex.hpp"
#include "parse.hpp"
#include "util.hpp"

int main() {
  std::string_view line = "int main main;\n";
  std::list<Token> list = {Token::lexLine(line)};

  Parser parser { std::move(list) };

  printContainer(parser._lexer._tokenlist);
  std::cout << '\n';


  while(!parser._lexer.endOfLexing())
  {
    [[maybe_unused]]auto curr { parser.parseExpr() };

    if(curr)
      std::cout << curr->toString() << '\n';
  }

  
  return 0;
}
