#pragma once

#include <iostream>
#include "lex.hpp"

template <typename T> void printContainer(T cont) {
  bool comma = false;
  std::cout << "{ ";

  for (const auto &e : cont) {
    if (comma)
      std::cout << ", ";
    else
      comma = true;

    std::cout << e;
  }

  std::cout << " }";
}

inline std::ostream &operator<<(std::ostream &out, const Token &token) {
  return out << Token::toString(token.type) << " : " << token.str;
}
