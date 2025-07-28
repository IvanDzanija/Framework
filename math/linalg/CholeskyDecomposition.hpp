#ifndef CHOLESKYDECOMPOSITION_H
#define CHOLESKYDECOMPOSITION_H

#pragma once
#include "Containers.hpp"
namespace math {

template <typename T> Matrix<T> Matrix<T>::decompose_Cholesky() {
    // O(n^3 / 3)
    // Works only for symmetric positive-definite matrices
    if (!this->is_symmetric()) {
        throw std::invalid_argument(
            "Matrix must be symmetric to try Cholesky decomposition!");
    }

    size_t n = this->cols_size();
    Matrix L(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            T sum = 0;
            for (size_t k = 0; k < j; ++k) {
                sum += L.at(i, k) * L.at(j, k);
            }
            if (i == j) {
                if (this->at(i, i) - sum <= 0) {
                    throw std::invalid_argument(
                        "Matrix is not positive definite!");
                }
                L.at(i, j) = std::sqrt(this->at(i, i) - sum);
            } else {
                L.at(i, j) = (this->at(i, j) - sum) / L.at(j, j);
            }
        }
    }
    return L;
}

} // namespace math

#endif // !CHOLESKYDECOMPOSITION_H
