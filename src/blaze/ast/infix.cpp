#include "./ast.hpp"

namespace blaze {
namespace ast {

infix::infix()
    : token{tok::type::ILLEGAL_, ""}
    , oper()
    , left()
    , right() {}

void infix::_expr() const {}

std::string infix::token_lit() const {
    return token.literal;
}

std::string infix::stringify() const {
    std::stringstream stream;
    stream << "(";
    stream << (left == nullptr ? "nil" : left->stringify());
    stream << " " + oper + " ";
    stream << (right == nullptr ? "nil" : right->stringify());
    stream << ")";
    return stream.str();
}

} // namespace ast
} // namespace blaze
