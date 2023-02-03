#include "./evaluator.hpp"
#include "../object/object.hpp"
#include <memory>

namespace blaze {

evaluator::evaluator(rand::generator_ &g, std::ostream &o)
    : stream(&o)
    , generator(g) {}

// NOLINTNEXTLINE(misc-no-recursion)
std::unique_ptr<obj::numeric_> evaluator::eval(ast::node_ &n, bool w) {
    if (auto *nr = dynamic_cast<ast::root *>(&n)) return eval(*nr->event, w);
    if (auto *ne = dynamic_cast<ast::roll *>(&n)) return eval(*ne->expr.get(), w);
    if (auto *ni = dynamic_cast<ast::integer *>(&n)) return eval_integer(*ni, w);
    if (auto *nf = dynamic_cast<ast::floating *>(&n)) return eval_floating(*nf, w);
    if (auto *nd = dynamic_cast<ast::dice *>(&n)) return eval_dice(*nd, w);
    if (auto *npx = dynamic_cast<ast::prefix *>(&n)) return eval_prefix(*npx, w);
    if (auto *nix = dynamic_cast<ast::infix *>(&n)) return eval_infix(*nix, w);
    throw exception("unable to evaluate (%1% %2%)", n.stringify(), n.token_lit());
}

std::unique_ptr<obj::numeric_> evaluator::eval_integer(ast::integer &ni, bool w) {
    auto oi = std::make_unique<obj::integer>(ni.value);
    if (w) *stream << format((ni.expl ? "(%1%)" : "%1%"), oi->inspect());
    return oi;
}

std::unique_ptr<obj::numeric_> evaluator::eval_floating(ast::floating &nf, bool w) {
    auto of = std::make_unique<obj::floating>(nf.value);
    if (w) *stream << format((nf.expl ? "(%1%)" : "%1%"), of->inspect());
    return of;
}

std::unique_ptr<obj::numeric_> evaluator::eval_dice(ast::dice &nd, bool w) {
    auto od = std::make_unique<obj::dice>(nd.n_dices, generator, nd.n_faces);
    if (w) *stream << format((nd.expl ? "(%1%)" : "%1%"), *od);
    return od;
}

// NOLINTNEXTLINE(misc-no-recursion)
std::unique_ptr<obj::numeric_> evaluator::eval_prefix(ast::prefix &npx, bool w) {
    if (w && npx.expl) *stream << "(";
    if (w) *stream << npx.token_lit();
    auto ro = eval(*npx.right, w);
    if (w && npx.expl) *stream << ")";
    return solve_prefix(npx.oper, *ro);
}

std::unique_ptr<obj::numeric_> evaluator::solve_prefix(std::string &op, obj::numeric_ &ro) {
    if (op == "-") return -ro;
    if (op == "+") return +ro;
    throw exception("(prefix) unknown operator (%1% %2%)", op, ro.inspect());
}

// NOLINTNEXTLINE(misc-no-recursion)
std::unique_ptr<obj::numeric_> evaluator::eval_infix(ast::infix &nix, bool w) {
    if (nix.oper == "^") return eval_infix_reps(nix);
    return eval_infix_math(nix, w);
}

// NOLINTNEXTLINE(misc-no-recursion)
std::unique_ptr<obj::numeric_> evaluator::eval_infix_math(ast::infix &nix, bool w) {
    if (w && nix.expl) *stream << "(";
    auto lo = eval(*nix.left, w);
    if (w) *stream << " " << nix.token_lit() << " ";
    auto ro = eval(*nix.right, w);
    if (w && nix.expl) *stream << ")";
    return solve_infix_math(nix.oper, *lo, *ro);
}

std::unique_ptr<obj::numeric_> evaluator::solve_infix_math(std::string &op, obj::numeric_ &lo, obj::numeric_ &ro) {
    if (op == "+") return lo + ro;
    if (op == "-") return lo - ro;
    if (op == "*") return lo * ro;
    if (op == "/") return lo / ro;
    throw exception("(infix) unknown operator (%1% %2% %3%)", lo.inspect(), op, ro.inspect());
}

// NOLINTNEXTLINE(misc-no-recursion)
std::unique_ptr<obj::numeric_> evaluator::eval_infix_reps(ast::infix &nix) {
    auto ro = eval(*nix.right, false);
    if (ro->type() != obj::type::INTEGER) {
        throw exception("(repetition) right operand != integer (... ^ %1%)", ro->type());
    }
    auto rv = dynamic_cast<obj::integer &>(*ro).value;
    if (rv <= 0) throw exception("(repetition) right operand <= 0 (... ^ %1%)", rv);
    auto sq = solve_infix_reps(rv, *nix.left);
    return sq;
}

// NOLINTNEXTLINE(misc-no-recursion)
std::unique_ptr<obj::numeric_> evaluator::solve_infix_reps(int64_t rv, ast::expr_ &ln) {
    auto sq = std::make_unique<obj::array>();
    for (auto i = 0; i < rv; ++i) {
        auto lo = eval(ln);
        if (lo->type() == obj::type::ARRAY) {
            throw exception("(repetition) left operand == array (%1% ^ ...)", lo->type());
        }
        *stream << " = " << lo->inspect() << std::endl;
        *sq << lo;
    }
    return sq;
}

} // namespace blaze
