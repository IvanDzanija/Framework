#include "Vector.hpp"

namespace maf::math {

template <typename T> void Vector<T>::fill(T value) noexcept {
    std::fill(_data.begin(), _data.end(), value);
}

template <typename T> [[nodiscard]] T Vector<T>::norm() const {
    T sum = 0.0;
    for (const T &val : _data) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

template <typename T> void Vector<T>::normalize() {
    // This will probably be faster by only calling norm() and checking if
    // norm is close to 0 since it only loops through the vector once! But
    // keeping this for clarity
    if (this->is_null()) {
        throw std::invalid_argument("Null vector can't be normalized!");
    }

    T norm = this->norm();
    for (T &val : _data) {
        val /= norm;
    }
}
template <typename T> Vector<T> Vector<T>::transposed() const noexcept {
    Orientation new_orientation = (_orientation == COLUMN) ? ROW : COLUMN;
    return ret(this->size(), _data, new_orientation);
}

} // namespace maf::math
