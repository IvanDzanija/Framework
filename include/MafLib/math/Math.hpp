#if defined(__APPLE__) && defined(ACCELERATE_AVAILABLE)
#include <vecLib/cblas_new.h>
#endif

#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/utility/UtilMath.hpp"

namespace maf::math {
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

}
