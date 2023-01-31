#pragma once
#include <iostream>

namespace Blaze {

enum class Precedence {
    LOWEST,
    SUM,
    PRODUCT,
    REPEAT,
    PREFIX,
};

std::ostream &operator<<(std::ostream &, const Precedence &);

} // namespace Blaze
