#include "./random.hpp"

namespace blaze {
namespace rand {

prng::prng(int64_t seed)
    : engine(seed) {}

uint64_t prng::spawn(uint64_t r) {
    return std::uniform_int_distribution<uint64_t>{1, r}(engine);
}

} // namespace rand
} // namespace blaze
