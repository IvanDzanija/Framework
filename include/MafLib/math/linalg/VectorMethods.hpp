#include "Vector.hpp"

namespace maf {

template <typename T> void Vector<T>::fill(T value) noexcept {
    std::fill(_data.begin(), _data.end(), value);
}

template <typename T> T Vector<T>::norm() const {
    T sum = 0.0;
    for (const T &val : _data) {
        sum += val * val;
    }
    return std::sqrt(sum);
}
template <typename T> Vector<T> Vector<T>::transposed() const noexcept {
    Orientation new_orientation = (_orientation == COLUMN) ? ROW : COLUMN;
    return ret(this->size(), _data, new_orientation);
}

} // namespace maf
