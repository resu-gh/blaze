#include "./ast.hpp"

namespace Blaze {
namespace AST {

Root::Root() {}

std::string Root::TokenLiteral() const {
    return (Event != nullptr) ? Event->TokenLiteral() : "nil";
}

std::string Root::String() const {
    return (Event != nullptr) ? Event->String() : "nil";
}

} // namespace AST
} // namespace Blaze
