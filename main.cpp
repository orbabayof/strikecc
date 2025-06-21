#include <iostream>
#include <string_view>

#include "lex.hpp"
#include "util.hpp"


int main() {
  std::string_view line = "int main main\n";
  std::list<Token> list = {Token::lexLine(line)};
  printContainer(list);
  std::cout << '\n';

  Lexer lex { std::move(list) };
  lex.nextToken();

  return 0;
}
