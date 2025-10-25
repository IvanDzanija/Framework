#pragma once
#include "Matrix.hpp"

namespace maf {

// Inplace fill
template <typename T> void Matrix<T>::fill(T value) {
    std::fill(_data.begin(), _data.end(), value);
}

// Set to identity
template <typename T> void Matrix<T>::make_identity() {
    if (!is_square()) {
        throw std::invalid_argument(
            "Only square matrices can be set to identity!");
    }

    fill(T(0));
    for (size_t i = 0; i < _rows; ++i) {
        this->at(i, i) = T(1);
    }
}

// Creates new transposed matrix
template <typename T> Matrix<T> Matrix<T>::transposed() const {
    Matrix<T> result(_cols, _rows);

#pragma omp parallel for if (_data.size() > 100 * 100)
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = 0; j < _cols; ++j) {
            result.at(j, i) = this->at(i, j);
        }
    }
    return result;
}

} // namespace maf
