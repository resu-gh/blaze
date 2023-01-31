#pragma once
#include "./format.hpp"

namespace Blaze {

template <typename... T> std::string format(const std::string &f, T &&...a) {
    return (boost::format(f) % ... % a).str();
}

} // namespace Blaze
