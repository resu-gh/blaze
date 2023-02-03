#include "./ast.hpp"

namespace blaze {
namespace ast {

integer::integer()
    : token{tok::type::ILLEGAL_, ""}
    , value(0) {}

void integer::_expr() const {}

std::string integer::token_lit() const {
    return token.literal;
}

std::string integer::stringify() const {
    return token.literal;
}

} // namespace ast
} // namespace blaze
