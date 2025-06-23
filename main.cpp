#include <iostream>
#include <print>
#include <string_view>

#include "lex.hpp"
#include "parse.hpp"
#include "util.hpp"
#include "gen.hpp"

int main() {
  std::string_view line = "return 0";
  std::list<Token> list = Token::lexLine(line);

  Parser parser { std::move(list) };

  printContainer(parser._tokenlist);
  std::cout << '\n';

  Generator gen { parser.parse() };

  std::cout << gen.generate() << '\n';
  

  return 0;
}
