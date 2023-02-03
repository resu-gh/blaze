#include "./token.hpp"

namespace blaze {
namespace tok {

std::ostream &operator<<(std::ostream &o, const type &t) {
    switch (t) {
    case type::ILLEGAL_: o << "tok::type::ILLEGAL_"; break;
    case type::EOF_: o << "tok::type::EOF_"; break;
    case type::PLUS: o << "tok::type::PLUS"; break;
    case type::MINUS: o << "tok::type::MINUS"; break;
    case type::STAR: o << "tok::type::STAR"; break;
    case type::SLASH: o << "tok::type::SLASH"; break;
    case type::DICE: o << "tok::type::DICE"; break;
    case type::INTEGER: o << "tok::type::INTEGER"; break;
    case type::FLOAT: o << "tok::type::FLOAT"; break;
    case type::LPAREN: o << "tok::type::LPAREN"; break;
    case type::RPAREN: o << "tok::type::RPAREN"; break;
    case type::CARET: o << "tok::type::CARET"; break;
    }
    return o;
}

std::ostream &operator<<(std::ostream &o, const token &k) {
    return o << "tok::token {" << k.type << " " << k.literal << "}";
}

} // namespace tok
} // namespace blaze
