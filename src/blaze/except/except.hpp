#pragma once
#include "../meta/format.hpp"
#include <exception>

namespace Blaze {

class Exception : public std::exception {
  private:
    std::string reason;

  public:
    template <typename... A> explicit Exception(const std::string &, A &&...);

    const char *what() const throw() override;
};

} // namespace Blaze

#include "./except.ipp"
