#pragma once // NOLINT(llvm-header-guard)
#include "../evaluator/evaluator.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../random/random.hpp"
#include <iostream>

namespace blaze {

class compiler {
  public:
    explicit compiler(const std::string &);
};

} // namespace blaze
