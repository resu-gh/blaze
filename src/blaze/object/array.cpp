#include "./object.hpp"

namespace blaze {
namespace obj {

array::array()
    : value(std::make_unique<integer>()) {}

type array::type() {
    return type::ARRAY;
}

std::string array::inspect() {
    return value->inspect();
}

std::unique_ptr<numeric_> array::operator+() {
    throw exception("invalid +(%1%)", type());
}

std::unique_ptr<numeric_> array::operator-() {
    throw exception("invalid -(%1%)", type());
}

std::unique_ptr<numeric_> array::operator+(numeric_ &n) {
    throw exception("invalid (%1% + %2%)", type(), n.type());
}

std::unique_ptr<numeric_> array::operator-(numeric_ &n) {
    throw exception("invalid (%1% - %2%)", type(), n.type());
}

std::unique_ptr<numeric_> array::operator*(numeric_ &n) {
    throw exception("invalid (%1% * %2%)", type(), n.type());
}

std::unique_ptr<numeric_> array::operator/(numeric_ &n) {
    throw exception("invalid (%1% / %2%)", type(), n.type());
}

void array::operator<<(std::unique_ptr<numeric_> &o) {
    value = *value + *o;
    values.push_back(std::move(o));
}

} // namespace obj
} // namespace blaze
