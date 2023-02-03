#include "./ast.hpp"

namespace blaze {
namespace ast {

root::root() {}

std::string root::token_lit() const {
    return (event != nullptr) ? event->token_lit() : "nil";
}

std::string root::stringify() const {
    return (event != nullptr) ? event->stringify() : "nil";
}

} // namespace ast
} // namespace blaze
