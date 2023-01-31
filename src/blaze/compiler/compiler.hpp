#pragma once
#include "../evaluator/evaluator.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../random/random.hpp"
#include <iostream>

namespace Blaze {

class Compiler {
  public:
    explicit Compiler(const std::string &);
};

} // namespace Blaze
