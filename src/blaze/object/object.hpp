#pragma once
#include "../except/except.hpp"
#include "../random/random.hpp"
#include <memory>
#include <sstream>

template <typename V> constexpr double dbl(V v) {
    return static_cast<double>(v);
}

class Generator;

namespace Blaze {
namespace OBJ {

enum class ObjectType {
    ERROR,
    INTEGER,
    FLOAT,
    DICE,
    SEQUENCE,
};

std::ostream &operator<<(std::ostream &, const ObjectType &);

class Object_ {
  public:
    Object_() = default;
    Object_(const Object_ &) = default;
    Object_(Object_ &&) = delete;
    Object_ &operator=(const Object_ &) = default;
    Object_ &operator=(Object_ &&) = delete;
    virtual ~Object_() = default;
    virtual ObjectType Type() = 0;
    virtual std::string Inspect() = 0;
};

class Numeric_ : public Object_ {
  public:
    Numeric_() = default;
    Numeric_(const Numeric_ &) = default;
    Numeric_(Numeric_ &&) = delete;
    Numeric_ &operator=(const Numeric_ &) = default;
    Numeric_ &operator=(Numeric_ &&) = delete;
    ~Numeric_() override = default;
    virtual std::unique_ptr<Numeric_> operator+() = 0;
    virtual std::unique_ptr<Numeric_> operator-() = 0;
    virtual std::unique_ptr<Numeric_> operator+(Numeric_ &) = 0;
    virtual std::unique_ptr<Numeric_> operator-(Numeric_ &) = 0;
    virtual std::unique_ptr<Numeric_> operator*(Numeric_ &) = 0;
    virtual std::unique_ptr<Numeric_> operator/(Numeric_ &) = 0;
};

class Integer final : public Numeric_ {
  public:
    int64_t Value;
    Integer();
    explicit Integer(int64_t);
    ObjectType Type() override;
    std::string Inspect() override;
    std::unique_ptr<Numeric_> operator+() override;
    std::unique_ptr<Numeric_> operator-() override;
    std::unique_ptr<Numeric_> operator+(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator-(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator*(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator/(Numeric_ &) override;
};

class Float final : public Numeric_ {
  public:
    double Value;
    Float();
    explicit Float(double);
    ObjectType Type() override;
    std::string Inspect() override;
    std::unique_ptr<Numeric_> operator+() override;
    std::unique_ptr<Numeric_> operator-() override;
    std::unique_ptr<Numeric_> operator+(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator-(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator*(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator/(Numeric_ &) override;
};

class Dice final : public Numeric_ {
  public:
    uint64_t Value;
    std::vector<uint64_t> Values;
    explicit Dice(uint64_t, Rand::Generator_ &, uint64_t);
    ObjectType Type() override;
    std::string Inspect() override;
    std::unique_ptr<Numeric_> operator+() override;
    std::unique_ptr<Numeric_> operator-() override;
    std::unique_ptr<Numeric_> operator+(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator-(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator*(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator/(Numeric_ &) override;
};

std::ostream &operator<<(std::ostream &, const Dice &);

class Sequence final : public Numeric_ {
  public:
    std::unique_ptr<Numeric_> Value;
    std::vector<std::unique_ptr<Numeric_>> Values;
    Sequence();
    ObjectType Type() override;
    std::string Inspect() override;
    std::unique_ptr<Numeric_> operator+() override;
    std::unique_ptr<Numeric_> operator-() override;
    std::unique_ptr<Numeric_> operator+(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator-(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator*(Numeric_ &) override;
    std::unique_ptr<Numeric_> operator/(Numeric_ &) override;
    void operator<<(std::unique_ptr<Numeric_> &o);
};

std::unique_ptr<Numeric_> trnc(double o);

} // namespace OBJ
} // namespace Blaze
