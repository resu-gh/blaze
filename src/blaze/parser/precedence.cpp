#include "./precedence.hpp"

namespace blaze {

std::ostream &operator<<(std::ostream &o, const precedence &p) {
    switch (p) {
    case precedence::LOWEST: o << "blaze::precedence::LOWEST"; break;
    case precedence::SUM: o << "blaze::precedence::SUM"; break;
    case precedence::PRODUCT: o << "blaze::precedence::PRODUCT"; break;
    case precedence::REPEAT: o << "blaze::precedence::REPEAT"; break;
    case precedence::PREFIX: o << "blaze::precedence::PREFIX"; break;
    }
    return o;
}

} // namespace blaze
