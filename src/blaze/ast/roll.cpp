#include "./ast.hpp"

namespace blaze {
namespace ast {

roll::roll()
    : token{tok::type::ILLEGAL_, ""}
    , expr() {}

void roll::_event() const {}

std::string roll::token_lit() const {
    return token.literal;
}

std::string roll::stringify() const {
    return (expr != nullptr) ? expr->stringify() : "nil";
}

} // namespace ast
} // namespace blaze
