#include "./random.hpp"

namespace Blaze {
namespace Rand {

PRNG::PRNG(int64_t seed)
    : engine(seed) {}

uint64_t PRNG::Generate(uint64_t r) {
    return std::uniform_int_distribution<uint64_t>{1, r}(engine);
}

} // namespace Rand
} // namespace Blaze
