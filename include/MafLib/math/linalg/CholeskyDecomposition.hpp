#ifndef CHOLESKYDECOMPOSITION_H
#define CHOLESKYDECOMPOSITION_H

#pragma once
#include "Matrix.hpp"
namespace maf {

/// Cholesky decomposition of symmetric positive definite matrix.
/// Symmetric positive definite matrix <=> has Cholesky decomposition.
template <typename T>
[[nodiscard]] Matrix<T> cholesky(const Matrix<T> &matrix) {
    // O(n^3 / 3)
    if (!matrix.is_symmetric()) {
        throw std::invalid_argument(
            "Matrix must be symmetric to try Cholesky decomposition!");
    }

    size_t n = matrix.row_count();
    Matrix<T> L(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            T sum = 0;
            for (size_t k = 0; k < j; ++k) {
                sum += L.at(i, k) * L.at(j, k);
            }
            if (i == j) {
                if (matrix.at(i, i) - sum <= 0) {
                    throw std::invalid_argument(
                        "Matrix is not positive definite!");
                }
                L.at(i, j) = std::sqrt(matrix.at(i, i) - sum);
            } else {
                L.at(i, j) = (matrix.at(i, j) - sum) / L.at(j, j);
            }
        }
    }
    return L;
}

} // namespace maf

#endif
