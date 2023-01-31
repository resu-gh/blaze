#pragma once
#include <boost/format.hpp>

namespace Blaze {

template <typename... T> std::string format(const std::string &f, T &&...a);

} // namespace Blaze

#include "./format.ipp"
