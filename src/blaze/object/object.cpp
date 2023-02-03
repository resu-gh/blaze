#include "./object.hpp"

namespace blaze {
namespace obj {

std::ostream &operator<<(std::ostream &o, const type &t) {
    switch (t) {
    case type::ERROR: o << "obj::type::ERROR"; break;
    case type::INTEGER: o << "obj::type::INTEGER"; break;
    case type::FLOAT: o << "obj::type::FLOAT"; break;
    case type::DICE: o << "obj::type::DICE"; break;
    case type::ARRAY: o << "obj::type::SEQUENCE"; break;
    }
    return o;
}

std::unique_ptr<numeric_> truncate_(double o) {
    if (trunc(o) == o) return std::make_unique<integer>(o);
    return std::make_unique<floating>(o);
}

} // namespace obj
} // namespace blaze
