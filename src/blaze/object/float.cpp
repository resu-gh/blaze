#include "./object.hpp"

namespace blaze {
namespace obj {

floating::floating()
    : values(0) {}

floating::floating(double v)
    : values(v) {}

type floating::type() {
    return type::FLOAT;
}

std::string floating::inspect() {
    std::string value = std::to_string(values);
    value.erase(value.find_last_not_of('0') + 1, std::string::npos);
    value.erase(value.find_last_not_of('.') + 1, std::string::npos);
    return value;
}

std::unique_ptr<numeric_> floating::operator+() {
    return std::make_unique<floating>(+values);
}

std::unique_ptr<numeric_> floating::operator-() {
    return std::make_unique<floating>(-values);
}

std::unique_ptr<numeric_> floating::operator+(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(values) + double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(values) + double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(values) + double_(r->value));
    throw exception("type mismatch (%1% + %2%)", type(), n.type());
}

std::unique_ptr<numeric_> floating::operator-(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(values) - double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(values) - double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(values) - double_(r->value));
    throw exception("type mismatch (%1% - %2%)", type(), n.type());
}

std::unique_ptr<numeric_> floating::operator*(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(values) * double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(values) * double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(values) * double_(r->value));
    throw exception("type mismatch (%1% * %2%)", type(), n.type());
}

std::unique_ptr<numeric_> floating::operator/(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) {
        if (r->value == 0) throw exception("division by zero (%1% / %2%)", inspect(), n.inspect());
        return truncate_(double_(values) / double_(r->value));
    }
    if (auto *r = dynamic_cast<floating *>(&n)) {
        if (r->values == 0) throw exception("division by zero (%1% / %2%)", inspect(), n.inspect());
        return truncate_(double_(values) / double_(r->values));
    }
    if (auto *r = dynamic_cast<dice *>(&n)) {
        if (r->value == 0) throw exception("division by zero (%1% / %2%)", inspect(), n.inspect());
        return truncate_(double_(values) / double_(r->value));
    }
    throw exception("type mismatch (%1% / %2%)", type(), n.type());
}

} // namespace obj
} // namespace blaze
