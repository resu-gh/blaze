#include "./object.hpp"

namespace Blaze {
namespace OBJ {

Dice::Dice(uint64_t nDices, Rand::Generator_ &g, uint64_t nFaces)
    : Value(0)
    , Values() {
    for (uint64_t i = 0; i < nDices; ++i) {
        uint64_t v = g.Generate(nFaces);
        Value += v;
        Values.push_back(v);
    }
}

ObjectType Dice::Type() {
    return ObjectType::DICE;
}

std::string Dice::Inspect() {
    return std::to_string(Value);
}

std::unique_ptr<Numeric_> Dice::operator+() {
    return std::make_unique<Integer>(+Value);
}

std::unique_ptr<Numeric_> Dice::operator-() {
    return std::make_unique<Integer>(-Value);
}

std::unique_ptr<Numeric_> Dice::operator+(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) + dbl(r->Value));
    throw Exception("Type mismatch (%1% + %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Dice::operator-(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) - dbl(r->Value));
    throw Exception("Type mismatch (%1% - %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Dice::operator*(Numeric_ &n) {
    if (auto *r = dynamic_cast<Integer *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    if (auto *r = dynamic_cast<Float *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    if (auto *r = dynamic_cast<Dice *>(&n)) return trnc(dbl(Value) * dbl(r->Value));
    throw Exception("Type mismatch (%1% * %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Dice::operator/(Numeric_ &n) {
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

std::ostream &operator<<(std::ostream &o, const Dice &d) {
    o << "[";
    for (auto value = d.Values.begin(); value != d.Values.end(); ++value) {
        o << *value << (value != d.Values.end() - 1 ? "," : "");
    }
    o << "]";
    return o << "(" << d.Value << ")";
}

} // namespace OBJ
} // namespace Blaze
