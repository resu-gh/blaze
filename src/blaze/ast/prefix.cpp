#include "./ast.hpp"

namespace Blaze {
namespace AST {

Prefix::Prefix()
    : Token{TokenType::ILLEGAL_, ""}
    , Operator()
    , Right() {}

void Prefix::_expr() const {}

std::string Prefix::TokenLiteral() const {
    return Token.Literal;
}

std::string Prefix::String() const {
    std::stringstream stream;
    stream << "(";
    stream << Operator;
    stream << (Right == nullptr ? "nil" : Right->String());
    stream << ")";
    return stream.str();
}

} // namespace AST
} // namespace Blaze
