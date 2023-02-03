#include "./ast.hpp"

namespace blaze {
namespace ast {

prefix::prefix()
    : token{tok::type::ILLEGAL_, ""}
    , oper()
    , right() {}

void prefix::_expr() const {}

std::string prefix::token_lit() const {
    return token.literal;
}

std::string prefix::stringify() const {
    std::stringstream stream;
    stream << "(";
    stream << oper;
    stream << (right == nullptr ? "nil" : right->stringify());
    stream << ")";
    return stream.str();
}

} // namespace ast
} // namespace blaze
