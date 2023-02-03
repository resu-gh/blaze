#include "./ast.hpp"

namespace blaze {
namespace ast {

floating::floating()
    : token{tok::type::ILLEGAL_, ""}
    , value(0) {}

void floating::_expr() const {}

std::string floating::token_lit() const {
    return token.literal;
}

std::string floating::stringify() const {
    return token.literal;
}

} // namespace ast
} // namespace blaze
