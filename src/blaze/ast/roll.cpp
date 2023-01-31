#include "./ast.hpp"

namespace Blaze {
namespace AST {

Roll::Roll()
    : Token{TokenType::ILLEGAL_, ""}
    , Expr() {}

void Roll::event_() const {}

std::string Roll::TokenLiteral() const {
    return Token.Literal;
}

std::string Roll::String() const {
    return (Expr != nullptr) ? Expr->String() : "nil";
}

} // namespace AST
} // namespace Blaze
