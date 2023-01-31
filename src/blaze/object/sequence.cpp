#include "./object.hpp"

namespace Blaze {
namespace OBJ {

Sequence::Sequence()
    : Value(std::make_unique<Integer>()) {}

ObjectType Sequence::Type() {
    return ObjectType::SEQUENCE;
}

std::string Sequence::Inspect() {
    return Value->Inspect();
}

std::unique_ptr<Numeric_> Sequence::operator+() {
    throw Exception("Invalid +(%1%)", Type());
}

std::unique_ptr<Numeric_> Sequence::operator-() {
    throw Exception("Invalid -(%1%)", Type());
}

std::unique_ptr<Numeric_> Sequence::operator+(Numeric_ &n) {
    throw Exception("Invalid (%1% + %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Sequence::operator-(Numeric_ &n) {
    throw Exception("Invalid (%1% - %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Sequence::operator*(Numeric_ &n) {
    throw Exception("Invalid (%1% * %2%)", Type(), n.Type());
}

std::unique_ptr<Numeric_> Sequence::operator/(Numeric_ &n) {
    throw Exception("Invalid (%1% / %2%)", Type(), n.Type());
}

void Sequence::operator<<(std::unique_ptr<Numeric_> &o) {
    Value = *Value + *o;
    Values.push_back(std::move(o));
}

} // namespace OBJ
} // namespace Blaze
