#pragma once // NOLINT(llvm-header-guard)
#include "./format.hpp"

namespace blaze {

template <typename... T> std::string format(const std::string &f, T &&...a) {
    return (boost::format(f) % ... % a).str();
}

} // namespace blaze
