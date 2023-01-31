#include "./ast.hpp"

namespace Blaze {
namespace AST {

Float::Float()
    : Token{TokenType::ILLEGAL_, ""}
    , Value(0) {}

void Float::_expr() const {}

std::string Float::TokenLiteral() const {
    return Token.Literal;
}

std::string Float::String() const {
    return Token.Literal;
}

} // namespace AST
} // namespace Blaze
