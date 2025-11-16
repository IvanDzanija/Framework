#include "MafLib/main/GlobalHeader.hpp"

namespace maf::math {
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

}
