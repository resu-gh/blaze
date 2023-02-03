#pragma once // NOLINT(llvm-header-guard)
#include "../except/except.hpp"
#include <openssl/rand.h>
#include <random>

namespace blaze {
namespace rand {

class generator_ {
  public:
    generator_() = default;
    generator_(const generator_ &) = default;
    generator_(generator_ &&) = delete;
    generator_ &operator=(const generator_ &) = default;
    generator_ &operator=(generator_ &&) = delete;
    virtual ~generator_() = default;
    virtual uint64_t spawn(uint64_t) = 0;
};

class prng : public generator_ {
  private:
    std::random_device device;
    std::mt19937_64 engine;

  public:
    explicit prng(int64_t seed);
    uint64_t spawn(uint64_t r) override;
};

class csprng : public generator_ {
  public:
    csprng();

    uint64_t spawn(uint64_t r) override;
};

} // namespace rand
} // namespace blaze
