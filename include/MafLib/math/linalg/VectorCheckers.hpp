#include "Vector.hpp"

namespace maf {
template <typename T>

bool Vector<T>::is_null() const noexcept {
    for (const T &val : _data) {
        if (!is_close(val, 0)) {
            return false;
        }
    }
    return true;
}

} // namespace maf
