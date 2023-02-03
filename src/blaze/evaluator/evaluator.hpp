#pragma once // NOLINT(llvm-header-guard)
#include "../ast/ast.hpp"
#include "../except/except.hpp"
#include "../object/object.hpp"

namespace blaze {

class evaluator final {
  private:
    std::ostream *stream;
    rand::generator_ &generator;

    std::unique_ptr<obj::numeric_> eval_integer(ast::integer &, bool = true);
    std::unique_ptr<obj::numeric_> eval_floating(ast::floating &, bool = true);
    std::unique_ptr<obj::numeric_> eval_dice(ast::dice &, bool = true);
    std::unique_ptr<obj::numeric_> eval_prefix(ast::prefix &, bool = true);
    std::unique_ptr<obj::numeric_> solve_prefix(std::string &, obj::numeric_ &);
    std::unique_ptr<obj::numeric_> eval_infix(ast::infix &, bool = true);
    std::unique_ptr<obj::numeric_> eval_infix_math(ast::infix &, bool = true);
    std::unique_ptr<obj::numeric_> solve_infix_math(std::string &, obj::numeric_ &, obj::numeric_ &);
    std::unique_ptr<obj::numeric_> eval_infix_reps(ast::infix &);
    std::unique_ptr<obj::numeric_> solve_infix_reps(int64_t, ast::expr_ &);

  public:
    explicit evaluator(rand::generator_ &, std::ostream &);

    std::unique_ptr<obj::numeric_> eval(ast::node_ &, bool = true);
};

} // namespace blaze
