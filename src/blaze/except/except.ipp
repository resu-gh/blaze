#pragma once
#include "./except.hpp"

namespace blaze {

template <typename... A>
exception::exception(const std::string &f, A &&...a)
    : reason(format(f, a...)) {}

} // namespace blaze
