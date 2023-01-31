#pragma once
#include "../ast/ast.hpp"
#include "../except/except.hpp"
#include "../object/object.hpp"

namespace Blaze {

class Evaluator final {
  private:
    std::ostream *stream;
    Rand::Generator_ &generator;

    std::unique_ptr<OBJ::Numeric_> evalInteger(AST::Integer &, bool = true);
    std::unique_ptr<OBJ::Numeric_> evalFloat(AST::Float &, bool = true);
    std::unique_ptr<OBJ::Numeric_> evalDice(AST::Dice &, bool = true);
    std::unique_ptr<OBJ::Numeric_> evalPrefix(AST::Prefix &, bool = true);
    std::unique_ptr<OBJ::Numeric_> solvePrefix(std::string &, OBJ::Numeric_ &);
    std::unique_ptr<OBJ::Numeric_> evalInfix(AST::Infix &, bool = true);
    std::unique_ptr<OBJ::Numeric_> evalMath(AST::Infix &, bool = true);
    std::unique_ptr<OBJ::Numeric_> solveMath(std::string &, OBJ::Numeric_ &, OBJ::Numeric_ &);
    std::unique_ptr<OBJ::Numeric_> evalReps(AST::Infix &);
    std::unique_ptr<OBJ::Numeric_> solveReps(int64_t, AST::Expr_ &);

  public:
    explicit Evaluator(Rand::Generator_ &, std::ostream &);

    std::unique_ptr<OBJ::Numeric_> Eval(AST::Node_ &, bool = true);
};

} // namespace Blaze
