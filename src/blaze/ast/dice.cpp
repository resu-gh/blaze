#include "./ast.hpp"

namespace blaze {
namespace ast {

dice::dice()
    : token{tok::type::ILLEGAL_, ""}
    , n_dices(0)
    , n_faces(0) {}

void dice::_expr() const {}

std::string dice::token_lit() const {
    return token.literal;
}

std::string dice::stringify() const {
    std::stringstream stream;
    stream << n_dices;
    stream << "d";
    stream << n_faces;
    return stream.str();
}

} // namespace ast
} // namespace blaze
