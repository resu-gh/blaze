#pragma once // NOLINT(llvm-header-guard)
#include "../meta/format.hpp"
#include <exception>

namespace blaze {

class exception : public std::exception {
  private:
    std::string reason;

  public:
    template <typename... A> explicit exception(const std::string &, A &&...);

    const char *what() const throw() override;
};

} // namespace blaze

#include "./except.ipp"
