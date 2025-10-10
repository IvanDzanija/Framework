#pragma once
#include "Matrix.hpp"

namespace maf {

template <typename T> bool Matrix<T>::is_square() const {
    return _rows == _cols;
}

template <typename T> bool Matrix<T>::is_symmetric() const {
    if (!is_square()) {
        throw std::invalid_argument("Matrix must be square to check symmetry.");
    }
    for (size_t i = 0; i < _rows; ++i) {
        for (size_t j = i + 1; j < _cols; ++j) {
            if (!is_close(this->at(i, j), this->at(j, i))) {
                return false;
            }
        }
    }
    return true;
}

template <typename T> bool Matrix<T>::is_upper_triangular() const {
    for (size_t i = 1; i < _rows; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (!is_close(this->at(i, j), static_cast<T>(0))) {
                return false;
            }
        }
    }
    return true;
}

template <typename T> bool Matrix<T>::is_lower_triangular() const {
    for (size_t i = 0; i < _rows - 1; ++i) {
        for (size_t j = i + 1; j < _cols; ++j) {
            if (!is_close(this->at(i, j), static_cast<T>(0))) {
                return false;
            }
        }
    }
    return true;
}

template <typename T> bool Matrix<T>::is_diagonal() const {
    if (!is_square()) {
        throw std::invalid_argument(
            "Matrix must be square to check diagonality.");
    }
    return is_upper_triangular() && is_lower_triangular();
}

template <typename T> bool Matrix<T>::is_singular() const {
    if (!is_square()) {
        throw std::invalid_argument("Non-square matrices are always singular.");
    }
    //  TODO: LOGIC
    //   For now, we will assume the matrix is not singular
    return false;
}
} // namespace maf
