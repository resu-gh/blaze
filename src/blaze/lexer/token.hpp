#pragma once
#include <iostream>

namespace blaze {
namespace tok {

enum class type {
    ILLEGAL_,
    EOF_,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    DICE,
    INTEGER,
    FLOAT,
    LPAREN,
    RPAREN,
    CARET,
};

std::ostream &operator<<(std::ostream &, const type &);

struct token final {
    tok::type type = type::ILLEGAL_;
    std::string literal = "";
};

std::ostream &operator<<(std::ostream &, const token &);

} // namespace tok
} // namespace blaze
