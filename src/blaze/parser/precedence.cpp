#include "./precedence.hpp"

namespace Blaze {

std::ostream &operator<<(std::ostream &o, const Precedence &p) {
    switch (p) {
    case Precedence::LOWEST: o << "Precedence::LOWEST"; break;
    case Precedence::SUM: o << "Precedence::SUM"; break;
    case Precedence::PRODUCT: o << "Precedence::PRODUCT"; break;
    case Precedence::REPEAT: o << "Precedence::REPEAT"; break;
    case Precedence::PREFIX: o << "Precedence::PREFIX"; break;
    }
    return o;
}

} // namespace Blaze
