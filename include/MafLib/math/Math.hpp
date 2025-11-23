
#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/utility/UtilMath.hpp"

namespace maf::math {
using namespace util;
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

}  // namespace maf::math
