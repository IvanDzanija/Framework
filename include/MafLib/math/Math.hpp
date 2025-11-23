
#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/utility/UtilMath.hpp"

namespace maf::math {
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

}
