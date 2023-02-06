#pragma once
#include <iostream>

namespace blaze {

enum class precedence {
    LOWEST,
    SUM,
    PRODUCT,
    REPEAT,
    PREFIX,
};

std::ostream &operator<<(std::ostream &, const precedence &);

} // namespace blaze
