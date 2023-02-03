#pragma once // NOLINT(llvm-header-guard)
#include "../except/except.hpp"
#include "../random/random.hpp"
#include <memory>
#include <sstream>

template <typename V> constexpr double double_(V v) {
    return static_cast<double>(v);
}

namespace blaze {
namespace obj {

enum class type {
    ERROR,
    INTEGER,
    FLOAT,
    DICE,
    ARRAY,
};

std::ostream &operator<<(std::ostream &, const type &);

class object_ {
  public:
    object_() = default;
    object_(const object_ &) = default;
    object_(object_ &&) = delete;
    object_ &operator=(const object_ &) = default;
    object_ &operator=(object_ &&) = delete;
    virtual ~object_() = default;
    virtual obj::type type() = 0;
    virtual std::string inspect() = 0;
};

class numeric_ : public object_ {
  public:
    numeric_() = default;
    numeric_(const numeric_ &) = default;
    numeric_(numeric_ &&) = delete;
    numeric_ &operator=(const numeric_ &) = default;
    numeric_ &operator=(numeric_ &&) = delete;
    ~numeric_() override = default;
    virtual std::unique_ptr<numeric_> operator+() = 0;
    virtual std::unique_ptr<numeric_> operator-() = 0;
    virtual std::unique_ptr<numeric_> operator+(numeric_ &) = 0;
    virtual std::unique_ptr<numeric_> operator-(numeric_ &) = 0;
    virtual std::unique_ptr<numeric_> operator*(numeric_ &) = 0;
    virtual std::unique_ptr<numeric_> operator/(numeric_ &) = 0;
};

class integer final : public numeric_ {
  public:
    int64_t value; // NOLINT(misc-non-private-member-variables-in-classes)
    integer();
    explicit integer(int64_t);
    obj::type type() override;
    std::string inspect() override;
    std::unique_ptr<numeric_> operator+() override;
    std::unique_ptr<numeric_> operator-() override;
    std::unique_ptr<numeric_> operator+(numeric_ &) override;
    std::unique_ptr<numeric_> operator-(numeric_ &) override;
    std::unique_ptr<numeric_> operator*(numeric_ &) override;
    std::unique_ptr<numeric_> operator/(numeric_ &) override;
};

class floating final : public numeric_ {
  public:
    double values; // NOLINT(misc-non-private-member-variables-in-classes)
    floating();
    explicit floating(double);
    obj::type type() override;
    std::string inspect() override;
    std::unique_ptr<numeric_> operator+() override;
    std::unique_ptr<numeric_> operator-() override;
    std::unique_ptr<numeric_> operator+(numeric_ &) override;
    std::unique_ptr<numeric_> operator-(numeric_ &) override;
    std::unique_ptr<numeric_> operator*(numeric_ &) override;
    std::unique_ptr<numeric_> operator/(numeric_ &) override;
};

class dice final : public numeric_ {
  public:
    uint64_t value;               // NOLINT(misc-non-private-member-variables-in-classes)
    std::vector<uint64_t> values; // NOLINT(misc-non-private-member-variables-in-classes)
    explicit dice(uint64_t, rand::generator_ &, uint64_t);
    obj::type type() override;
    std::string inspect() override;
    std::unique_ptr<numeric_> operator+() override;
    std::unique_ptr<numeric_> operator-() override;
    std::unique_ptr<numeric_> operator+(numeric_ &) override;
    std::unique_ptr<numeric_> operator-(numeric_ &) override;
    std::unique_ptr<numeric_> operator*(numeric_ &) override;
    std::unique_ptr<numeric_> operator/(numeric_ &) override;
};

std::ostream &operator<<(std::ostream &, const dice &);

class array final : public numeric_ {
  public:
    std::unique_ptr<numeric_> value;               // NOLINT(misc-non-private-member-variables-in-classes)
    std::vector<std::unique_ptr<numeric_>> values; // NOLINT(misc-non-private-member-variables-in-classes)
    array();
    obj::type type() override;
    std::string inspect() override;
    std::unique_ptr<numeric_> operator+() override;
    std::unique_ptr<numeric_> operator-() override;
    std::unique_ptr<numeric_> operator+(numeric_ &) override;
    std::unique_ptr<numeric_> operator-(numeric_ &) override;
    std::unique_ptr<numeric_> operator*(numeric_ &) override;
    std::unique_ptr<numeric_> operator/(numeric_ &) override;
    void operator<<(std::unique_ptr<numeric_> &o);
};

std::unique_ptr<numeric_> truncate_(double o);

} // namespace obj
} // namespace blaze
