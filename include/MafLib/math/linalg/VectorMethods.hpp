#include "Vector.hpp"

namespace maf {

template <typename T> Vector<T> Vector<T>::transposed() const noexcept {
    Orientation new_orientation = (_orientation == COLUMN) ? ROW : COLUMN;
    return ret(this->size(), _data, new_orientation);
}

} // namespace maf
