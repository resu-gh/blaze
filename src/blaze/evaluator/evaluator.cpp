#include "./evaluator.hpp"
#include "../object/object.hpp"
#include <memory>

namespace Blaze {

Evaluator::Evaluator(Rand::Generator_ &g, std::ostream &o)
    : stream(&o)
    , generator(g) {}

std::unique_ptr<OBJ::Numeric_> Evaluator::Eval(AST::Node_ &n, bool w) {
    if (auto *nr = dynamic_cast<AST::Root *>(&n)) return Eval(*nr->Event, w);
    if (auto *ne = dynamic_cast<AST::Roll *>(&n)) return Eval(*ne->Expr.get(), w);
    if (auto *ni = dynamic_cast<AST::Integer *>(&n)) return evalInteger(*ni, w);
    if (auto *nf = dynamic_cast<AST::Float *>(&n)) return evalFloat(*nf, w);
    if (auto *nd = dynamic_cast<AST::Dice *>(&n)) return evalDice(*nd, w);
    if (auto *npx = dynamic_cast<AST::Prefix *>(&n)) return evalPrefix(*npx, w);
    if (auto *nix = dynamic_cast<AST::Infix *>(&n)) return evalInfix(*nix, w);
    throw Exception("Unable to Evaluate (%1% %2%)", n.String(), n.TokenLiteral());
}

std::unique_ptr<OBJ::Numeric_> Evaluator::evalInteger(AST::Integer &ni, bool w) {
    auto oi = std::make_unique<OBJ::Integer>(ni.Value);
    if (w) *stream << format((ni.expl ? "(%1%)" : "%1%"), oi->Inspect());
    return oi;
}

std::unique_ptr<OBJ::Numeric_> Evaluator::evalFloat(AST::Float &nf, bool w) {
    auto of = std::make_unique<OBJ::Float>(nf.Value);
    if (w) *stream << format((nf.expl ? "(%1%)" : "%1%"), of->Inspect());
    return of;
}

std::unique_ptr<OBJ::Numeric_> Evaluator::evalDice(AST::Dice &nd, bool w) {
    auto od = std::make_unique<OBJ::Dice>(nd.nDices, generator, nd.nFaces);
    if (w) *stream << format((nd.expl ? "(%1%)" : "%1%"), *od);
    return od;
}

std::unique_ptr<OBJ::Numeric_> Evaluator::evalPrefix(AST::Prefix &npx, bool w) {
    if (w && npx.expl) *stream << "(";
    if (w) *stream << npx.TokenLiteral();
    auto ro = Eval(*npx.Right, w);
    if (w && npx.expl) *stream << ")";
    return solvePrefix(npx.Operator, *ro);
}

std::unique_ptr<OBJ::Numeric_> Evaluator::solvePrefix(std::string &op, OBJ::Numeric_ &ro) {
    if (op == "-") return -ro;
    if (op == "+") return +ro;
    throw Exception("(Prefix) Unknown operator (%1% %2%)", op, ro.Inspect());
}

std::unique_ptr<OBJ::Numeric_> Evaluator::evalInfix(AST::Infix &nix, bool w) {
    if (nix.Operator == "^") return evalReps(nix);
    return evalMath(nix, w);
}

std::unique_ptr<OBJ::Numeric_> Evaluator::evalMath(AST::Infix &nix, bool w) {
    if (w && nix.expl) *stream << "(";
    auto lo = Eval(*nix.Left, w);
    if (w) *stream << " " << nix.TokenLiteral() << " ";
    auto ro = Eval(*nix.Right, w);
    if (w && nix.expl) *stream << ")";
    return solveMath(nix.Operator, *lo, *ro);
}

std::unique_ptr<OBJ::Numeric_> Evaluator::solveMath(std::string &op, OBJ::Numeric_ &lo, OBJ::Numeric_ &ro) {
    if (op == "+") return lo + ro;
    if (op == "-") return lo - ro;
    if (op == "*") return lo * ro;
    if (op == "/") return lo / ro;
    throw Exception("(Infix) Unknown operator (%1% %2% %3%)", lo.Inspect(), op, ro.Inspect());
}

std::unique_ptr<OBJ::Numeric_> Evaluator::evalReps(AST::Infix &nix) {
    auto ro = Eval(*nix.Right, false);
    if (ro->Type() != OBJ::ObjectType::INTEGER) {
        throw Exception("(Repetition) Right operand != Integer (... ^ %1%)", ro->Type());
    }
    auto rv = dynamic_cast<OBJ::Integer &>(*ro).Value;
    if (rv <= 0) throw Exception("(Repetition) Right operand <= 0 (... ^ %1%)", rv);
    auto sq = solveReps(rv, *nix.Left);
    return sq;
}

std::unique_ptr<OBJ::Numeric_> Evaluator::solveReps(int64_t rv, AST::Expr_ &ln) {
    auto sq = std::make_unique<OBJ::Sequence>();
    for (auto i = 0; i < rv; ++i) {
        auto lo = Eval(ln);
        if (lo->Type() == OBJ::ObjectType::SEQUENCE) {
            throw Exception("(Repetition) Left operand == Sequence (%1% ^ ...)", lo->Type());
        }
        *stream << " = " << lo->Inspect() << std::endl;
        *sq << lo;
    }
    return sq;
}

} // namespace Blaze
