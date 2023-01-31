#include "./except.hpp"

namespace Blaze {

const char *Exception::what() const throw() {
    return reason.c_str();
}

} // namespace Blaze
