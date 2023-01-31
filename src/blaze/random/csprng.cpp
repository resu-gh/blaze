#include "./random.hpp"

namespace Blaze {
namespace Rand {

CSPRNG::CSPRNG(){};

uint64_t CSPRNG::Generate(uint64_t r) {
    uint64_t x = 0;
    auto ret = RAND_bytes(reinterpret_cast<unsigned char *>(&x), sizeof x);
    if (ret == 0) throw Exception("CSRNG Error: Low Entropy");
    return (x % r) + 1;
}

} // namespace Rand
} // namespace Blaze
