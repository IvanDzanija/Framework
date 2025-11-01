#ifndef MATRIX_CHECKERS_H
#define MATRIX_CHECKERS_H
#pragma once
#include "Matrix.hpp"

/**
 * @file MatrixCheckers.hpp
 * @brief Contains implementations for the checker methods of the Matrix<T>
 * class.
 *
 * This file is intended to be included at the *end* of Matrix.hpp and
 * should not be included directly anywhere else.
 */
namespace maf::math {
// Checks if matrix is square.
template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_square() const {
    return _rows == _cols;
}

// Checks if matrix is symmetric.
template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_symmetric() const {
    if (!is_square()) {
        return false;
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

// Checks if matrix is upper triangular.
template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_upper_triangular() const {
    if (!is_square()) {
        return false;
    }
    for (size_t i = 1; i < _rows; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (!is_close(this->at(i, j), static_cast<T>(0))) {
                return false;
            }
        }
    }
    return true;
}

// Checks if matrix is lower triangular.
template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_lower_triangular() const {
    if (!is_square()) {
        return false;
    }
    for (size_t i = 0; i < _rows - 1; ++i) {
        for (size_t j = i + 1; j < _cols; ++j) {
            if (!is_close(this->at(i, j), static_cast<T>(0))) {
                return false;
            }
        }
    }
    return true;
}

// Checks if matrix is diagonal.
template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_diagonal() const {
    if (!is_square()) {
        return false;
    }
    return is_upper_triangular() && is_lower_triangular();
}

// Checks if matrix is singular.
template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_singular() const {
    if (!is_square()) {
        return true;
    }
    try {
        plu(*this);
        return false;
    } catch (const std::runtime_error &e) {
        return true;
    }
}

template <typename T>
[[nodiscard]] bool Matrix<T>::is_positive_definite() const {
    try {
        cholesky(*this);
        return true;
    } catch (std::exception e) {
        return false;
    }
}

} // namespace maf::math

#endif
