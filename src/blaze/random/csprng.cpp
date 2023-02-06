#include "./random.hpp"

namespace blaze {
namespace rand {

csprng::csprng(){};

uint64_t csprng::spawn(uint64_t r) {
    uint64_t x = 0;
    auto ret = RAND_bytes(reinterpret_cast<unsigned char *>(&x), sizeof x);
    if (ret == 0) throw exception("blaze::rand::csrng error: low entropy");
    return (x % r) + 1;
}

} // namespace rand
} // namespace blaze
