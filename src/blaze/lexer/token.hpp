#pragma once
#include <iostream>

namespace Blaze {

enum class TokenType {
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

std::ostream &operator<<(std::ostream &, const TokenType &);

struct Token final {
    TokenType Type = TokenType::ILLEGAL_;
    std::string Literal = "";
};

std::ostream &operator<<(std::ostream &, const Token &);

} // namespace Blaze
