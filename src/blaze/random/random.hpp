#pragma once
#include "../except/except.hpp"
#include <openssl/rand.h>
#include <random>

namespace Blaze {
namespace Rand {

class Generator_ {
  public:
    Generator_() = default;
    Generator_(const Generator_ &) = default;
    Generator_(Generator_ &&) = delete;
    Generator_ &operator=(const Generator_ &) = default;
    Generator_ &operator=(Generator_ &&) = delete;
    virtual ~Generator_() = default;
    virtual uint64_t Generate(uint64_t) = 0;
};

class PRNG : public Generator_ {
  private:
    std::random_device device;
    std::mt19937_64 engine;

  public:
    explicit PRNG(int64_t seed);
    uint64_t Generate(uint64_t r) override;
};

class CSPRNG : public Generator_ {
  public:
    CSPRNG();

    uint64_t Generate(uint64_t r) override;
};

} // namespace Rand
} // namespace Blaze
