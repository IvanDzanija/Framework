#ifndef CHOLESKYDECOMPOSITION_H
#define CHOLESKYDECOMPOSITION_H
#pragma once
#include "Matrix.hpp"

namespace maf::math {

/// Cholesky decomposition of symmetric positive definite matrix.
/// Symmetric positive definite matrix <=> has Cholesky decomposition.
template <typename T>
[[nodiscard]] Matrix<T> cholesky(const Matrix<T> &matrix) {
    if (!matrix.is_symmetric()) {
        throw std::invalid_argument(
            "Matrix must be symmetric to try Cholesky decomposition!");
    }

    size_t n = matrix.row_count();
    Matrix<T> L(n, n);
    auto D = matrix.transposed();

    for (size_t jj = 0; jj < n; jj += BLOCK_SIZE) {
        const size_t j_end = std::min(jj + BLOCK_SIZE, n);
        for (size_t j = jj; j < j_end; ++j) {
            T sum = 0;
            auto L_row_j = L.row_span(j);
#pragma omp simd
            for (size_t k = 0; k < j; ++k) {
                sum += L_row_j[k] * L_row_j[k];
            }

            T diag_val = matrix.at(j, j) - sum;
            if (diag_val <= 0) {
                throw std::invalid_argument("Matrix is not positive definite!");
            }
            L_row_j[j] = std::sqrt(diag_val);

            for (size_t i = j + 1; i < j_end; ++i) {
                T sum_i = 0;
                auto L_row_i = L.row_span(i);
#pragma omp simd
                for (size_t k = 0; k < j; ++k) {
                    sum_i += L_row_i[k] * L_row_j[k];
                }
                L_row_i[j] = (matrix.at(i, j) - sum_i) / L_row_j[j];
            }
        }

#pragma omp parallel for if (n > 1000)
        for (size_t ii = j_end; ii < n; ii += BLOCK_SIZE) {
            const size_t i_end = std::min(ii + BLOCK_SIZE, n);

            for (size_t i = ii; i < i_end; ++i) {
                auto L_row_i = L.row_span(i);

                for (size_t j = jj; j < j_end; ++j) {
                    T sum = 0;
                    auto L_row_j = L.row_span(j);

#pragma omp simd
                    for (size_t k = 0; k < j; ++k) {
                        sum += L_row_i[k] * L_row_j[k];
                    }
                    L_row_i[j] = (matrix.at(i, j) - sum) / L_row_j[j];
                }
            }
        }
    }

    return L;
}

} // namespace maf::math
#endif
