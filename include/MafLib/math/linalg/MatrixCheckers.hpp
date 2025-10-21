#pragma once
#include "MafLib/math/linalg/CholeskyDecomposition.hpp"
#include "Matrix.hpp"
#include <exception>

namespace maf {

template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_square() const {
    return _rows == _cols;
}

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

template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_diagonal() const {
    if (!is_square()) {
        return false;
    }
    return is_upper_triangular() && is_lower_triangular();
}

template <typename T>
[[nodiscard]] constexpr bool Matrix<T>::is_singular() const {
    if (!is_square()) {
        return false;
    }
    //  TODO: LOGIC
    //   For now, we will assume the matrix is not singular
    return false;
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

} // namespace maf
