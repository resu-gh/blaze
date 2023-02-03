#include "./object.hpp"

namespace blaze {
namespace obj {

dice::dice(uint64_t nd, rand::generator_ &g, uint64_t nf)
    : value(0)
    , values() {
    for (uint64_t i = 0; i < nd; ++i) {
        const uint64_t v = g.spawn(nf);
        value += v;
        values.push_back(v);
    }
}

type dice::type() {
    return type::DICE;
}

std::string dice::inspect() {
    return std::to_string(value);
}

std::unique_ptr<numeric_> dice::operator+() {
    return std::make_unique<integer>(+value);
}

std::unique_ptr<numeric_> dice::operator-() {
    return std::make_unique<integer>(-value);
}

std::unique_ptr<numeric_> dice::operator+(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(value) + double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(value) + double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(value) + double_(r->value));
    throw exception("type mismatch (%1% + %2%)", type(), n.type());
}

std::unique_ptr<numeric_> dice::operator-(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(value) - double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(value) - double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(value) - double_(r->value));
    throw exception("type mismatch (%1% - %2%)", type(), n.type());
}

std::unique_ptr<numeric_> dice::operator*(numeric_ &n) {
    if (auto *r = dynamic_cast<integer *>(&n)) return truncate_(double_(value) * double_(r->value));
    if (auto *r = dynamic_cast<floating *>(&n)) return truncate_(double_(value) * double_(r->values));
    if (auto *r = dynamic_cast<dice *>(&n)) return truncate_(double_(value) * double_(r->value));
    throw exception("type mismatch (%1% * %2%)", type(), n.type());
}

std::unique_ptr<numeric_> dice::operator/(numeric_ &n) {
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

std::ostream &operator<<(std::ostream &o, const dice &d) {
    o << "[";
    for (auto value = d.values.begin(); value != d.values.end(); ++value) {
        o << *value << (value != d.values.end() - 1 ? "," : "");
    }
    o << "]";
    return o << "(" << d.value << ")";
}

} // namespace obj
} // namespace blaze
