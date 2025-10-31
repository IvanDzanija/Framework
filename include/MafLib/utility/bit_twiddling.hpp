#ifndef BIT_TWIDDLING_H
#define BIT_TWIDDLING_H
#include <cmath>

namespace maf::util {

inline bool same_sign(int a, int b) { return ((a ^ b) >= 0); }

inline bool is_power_of_two(int n) { return (n > 0) && ((n & (n - 1)) == 0); }

inline unsigned int most_significant_bit(int n) {
    auto d = static_cast<double>(n);
    return (n == 0) ? 0 : std::ilogb(d) + 1;
}

} // namespace maf::util
#endif // BIT_TWIDDLING_H
