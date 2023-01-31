#include "./ast.hpp"

namespace Blaze {
namespace AST {

Integer::Integer()
    : Token{TokenType::ILLEGAL_, ""}
    , Value(0) {}

void Integer::_expr() const {}

std::string Integer::TokenLiteral() const {
    return Token.Literal;
}

std::string Integer::String() const {
    return Token.Literal;
}

} // namespace AST
} // namespace Blaze
