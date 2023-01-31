#include "./object.hpp"

namespace Blaze {
namespace OBJ {

Integer::Integer()
    : Value(0) {}

Integer::Integer(int64_t v)
    : Value(v) {}

ObjectType Integer::Type() {
    return ObjectType::INTEGER;
}

std::string Integer::Inspect() {
    return std::to_string(Value);
}

std::unique_ptr<Numeric_> Integer::operator+() {
    return std::make_unique<Integer>(+Value);
}

std::unique_ptr<Numeric_> Integer::operator-() {
    return std::make_unique<Integer>(-Value);
}

std::unique_ptr<Numeric_> Integer::operator+(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    throw Exception("Type mismatch (%1% + %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Integer::operator-(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    throw Exception("Type mismatch (%1% - %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Integer::operator*(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    throw Exception("Type mismatch (%1% * %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Integer::operator/(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) {
        if (r->Value == 0) throw Exception("Division by zero (%1% / %2%)", Inspect(), n.Inspect());
        return trnc(dbl(Value) / dbl(r->Value));
    }
    if (auto *r = dynamic_cast<Float *>(&n)) {
        if (r->Value == 0) throw Exception("Division by zero (%1% / %2%)", Inspect(), n.Inspect());
        return trnc(dbl(Value) / dbl(r->Value));
    }
    if (auto *r = dynamic_cast<Dice *>(&n)) {
        if (r->Value == 0) throw Exception("Division by zero (%1% / %2%)", Inspect(), n.Inspect());
        return trnc(dbl(Value) / dbl(r->Value));
    }
    throw Exception("Type mismatch (%1% / %2%)", Type(), n.Type());
}

} // namespace OBJ
} // namespace Blaze
