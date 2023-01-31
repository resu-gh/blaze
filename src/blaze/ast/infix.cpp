#include "./ast.hpp"

namespace Blaze {
namespace AST {

Infix::Infix()
    : Token{TokenType::ILLEGAL_, ""}
    , Operator()
    , Left()
    , Right() {}

void Infix::_expr() const {}

std::string Infix::TokenLiteral() const {
    return Token.Literal;
}

std::string Infix::String() const {
    std::stringstream stream;
    stream << "(";
    stream << (Left == nullptr ? "nil" : Left->String());
    stream << " " + Operator + " ";
    stream << (Right == nullptr ? "nil" : Right->String());
    stream << ")";
    return stream.str();
}

} // namespace AST
} // namespace Blaze
