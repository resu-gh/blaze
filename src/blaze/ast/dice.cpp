#include "./ast.hpp"

namespace Blaze {
namespace AST {

Dice::Dice()
    : Token{TokenType::ILLEGAL_, ""}
    , nDices(0)
    , nFaces(0) {}

void Dice::_expr() const {}

std::string Dice::TokenLiteral() const {
    return Token.Literal;
}

std::string Dice::String() const {
    std::stringstream stream;
    stream << nDices;
    stream << "d";
    stream << nFaces;
    return stream.str();
}

} // namespace AST
} // namespace Blaze
