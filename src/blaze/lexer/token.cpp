#include "./token.hpp"

namespace Blaze {

std::ostream &operator<<(std::ostream &o, const TokenType &t) {
    switch (t) {
    case TokenType::ILLEGAL_: o << "TokenType::ILLEGAL_"; break;
    case TokenType::EOF_: o << "TokenType::EOF_"; break;
    case TokenType::PLUS: o << "TokenType::PLUS"; break;
    case TokenType::MINUS: o << "TokenType::MINUS"; break;
    case TokenType::STAR: o << "TokenType::STAR"; break;
    case TokenType::SLASH: o << "TokenType::SLASH"; break;
    case TokenType::DICE: o << "TokenType::DICE"; break;
    case TokenType::INTEGER: o << "TokenType::INTEGER"; break;
    case TokenType::FLOAT: o << "TokenType::FLOAT"; break;
    case TokenType::LPAREN: o << "TokenType::LPAREN"; break;
    case TokenType::RPAREN: o << "TokenType::RPAREN"; break;
    case TokenType::CARET: o << "TokenType::CARET"; break;
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const Token &k) {
    return o << "Token {" << k.Type << " " << k.Literal << "}";
}

} // namespace Blaze
