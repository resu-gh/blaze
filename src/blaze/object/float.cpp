#include "./object.hpp"

namespace Blaze {
namespace OBJ {

Float::Float()
    : Value(0) {}

Float::Float(double v)
    : Value(v) {}

ObjectType Float::Type() {
    return ObjectType::FLOAT;
}

std::string Float::Inspect() {
    std::string value = std::to_string(Value);
    value.erase(value.find_last_not_of('0') + 1, std::string::npos);
    value.erase(value.find_last_not_of('.') + 1, std::string::npos);
    return value;
}

std::unique_ptr<Numeric_> Float::operator+() {
    return std::make_unique<Float>(+Value);
}

std::unique_ptr<Numeric_> Float::operator-() {
    return std::make_unique<Float>(-Value);
}

std::unique_ptr<Numeric_> Float::operator+(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    throw Exception("Type mismatch (%1% + %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Float::operator-(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    throw Exception("Type mismatch (%1% - %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Float::operator*(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    throw Exception("Type mismatch (%1% * %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Float::operator/(Numeric_ &n) {
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
