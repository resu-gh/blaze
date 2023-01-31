#pragma once
#include "./except.hpp"

namespace Blaze {

template <typename... A>
Exception::Exception(const std::string &f, A &&...a)
    : reason(format(f, a...)) {}

} // namespace Blaze
