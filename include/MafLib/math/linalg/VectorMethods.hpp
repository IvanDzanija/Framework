#ifndef VECTOR_METHODS_H
#define VECTOR_METHODS_H
#pragma once
#include "Vector.hpp"

namespace maf::math {
// Inplace fill
template <Numeric T>
void Vector<T>::fill(T value) noexcept {
    std::fill(_data.begin(), _data.end(), value);
}

// L2 Norm
template <Numeric T>
[[nodiscard]] T Vector<T>::norm() const noexcept {
    T sum = 0.0;
    for (const T& val : _data) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

// Inplace normalize with L2 norm
template <Numeric T>
void Vector<T>::normalize() {
    T norm = this->norm();
    if (is_close(norm, 0)) {
        throw std::invalid_argument("Vector norm is close to 0!");
    }

    for (T& val : _data) {
        val /= norm;
    }
}

// Inplace transpose
template <Numeric T>
void Vector<T>::transpose() noexcept {
    _orientation = (_orientation == COLUMN) ? ROW : COLUMN;
}

// Creates new transposed vector
template <Numeric T>
[[nodiscard]] Vector<T> Vector<T>::transposed() const noexcept {
    Orientation new_orientation = (_orientation == COLUMN) ? ROW : COLUMN;
    return Vector<T>(this->size(), _data, new_orientation);
}

}  // namespace maf::math

#endif
