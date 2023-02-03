#pragma once // NOLINT(llvm-header-guard)
#include <boost/format.hpp>

namespace blaze {

template <typename... T> std::string format(const std::string &f, T &&...a);

} // namespace blaze

#include "./format.ipp"
