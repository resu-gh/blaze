#include "./object.hpp"

namespace Blaze {
namespace OBJ {

std::ostream &operator<<(std::ostream &o, const ObjectType &t) {
    switch (t) {
    case ObjectType::ERROR: o << "ObjectType::ERROR"; break;
    case ObjectType::INTEGER: o << "ObjectType::INTEGER"; break;
    case ObjectType::FLOAT: o << "ObjectType::FLOAT"; break;
    case ObjectType::DICE: o << "ObjectType::DICE"; break;
    case ObjectType::SEQUENCE: o << "ObjectType::SEQUENCE"; break;
    }
    return o;
}

std::unique_ptr<Numeric_> trnc(double o) {
    if (trunc(o) == o) return std::make_unique<Integer>(o);
    return std::make_unique<Float>(o);
}

} // namespace OBJ
} // namespace Blaze
