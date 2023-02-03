#include "./except.hpp"

namespace blaze {

const char *exception::what() const throw() {
    return reason.c_str();
}

} // namespace blaze
