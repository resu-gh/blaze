#include "./object.hpp"

namespace blaze {
namespace obj {

integer::integer()
    : value(0) {}

integer::integer(int64_t v)
    : value(v) {}

type integer::type() {
    return type::INTEGER;
}

std::string integer::inspect() {
    return std::to_string(value);
}

std::unique_ptr<numeric_> integer::operator+() {
    return std::make_unique<integer>(+value);
}

std::unique_ptr<numeric_> integer::operator-() {
    return std::make_unique<integer>(-value);
}

std::unique_ptr<numeric_> integer::operator+(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(value) + double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(value) + double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(value) + double_(r->value));
    throw exception("type mismatch (%1% + %2%)", type(), n.type());
}

std::unique_ptr<numeric_> integer::operator-(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(value) - double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(value) - double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(value) - double_(r->value));
    throw exception("type mismatch (%1% - %2%)", type(), n.type());
}

std::unique_ptr<numeric_> integer::operator*(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(value) * double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(value) * double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(value) * double_(r->value));
    throw exception("type mismatch (%1% * %2%)", type(), n.type());
}

std::unique_ptr<numeric_> integer::operator/(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) {
        if (r->value == 0) throw exception("division by zero (%1% / %2%)", inspect(), n.inspect());
        return truncate_(double_(value) / double_(r->value));
    }
    if (auto *r = dynamic_cast<floating *>(&n)) {
        if (r->values == 0) throw exception("division by zero (%1% / %2%)", inspect(), n.inspect());
        return truncate_(double_(value) / double_(r->values));
    }
    if (auto *r = dynamic_cast<dice *>(&n)) {
        if (r->value == 0) throw exception("division by zero (%1% / %2%)", inspect(), n.inspect());
        return truncate_(double_(value) / double_(r->value));
    }
    throw exception("type mismatch (%1% / %2%)", type(), n.type());
}

} // namespace obj
} // namespace blaze
