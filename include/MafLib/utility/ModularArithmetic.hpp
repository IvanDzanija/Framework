#include "MafLib/main/GlobalHeader.hpp"

namespace maf::util {
// TODO: Make T integral type only
template <typename T, T Mod>
class ModInt {
public:
    [[nodiscard]] T get_value() {
        return _val;
    }
    [[nodiscard]] T get_mod() {
        return MOD;
    }

private:
    T _val;
    constexpr static T MOD = Mod;
};
}  // namespace maf::util
