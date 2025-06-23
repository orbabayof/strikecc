#pragma once

#include "ast.hpp"
#include <string>

class Generator 
{
public:
  Generator(Program p): _program { p } {}

  std::string generate();
private:
  Program _program {};
};
