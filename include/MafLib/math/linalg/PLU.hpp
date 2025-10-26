#ifndef MUTRIX_PLU_H
#define MUTRIX_PLU_H

#include <algorithm>
#pragma once
#include "Matrix.hpp"
namespace maf::math {

/// PLU decomposition of square matrix.
/// P - permutation, L - lower triangular, U - upper triangular
/// More information:
/// https://en.wikipedia.org/wiki/LU_decomposition#LU_factorization_with_partial_pivoting
/// P is saved as a std::vector of fixed size with the final row order!
/// @return Tuple of std::vector P, Matrix L, Matrix U.
template <typename T>
[[nodiscard]] std::tuple<std::vector<size_t>, Matrix<T>, Matrix<T>>
plu(const Matrix<T> &matrix) {
    if (!matrix.is_square()) {
        throw std::invalid_argument(
            "Matrix must be square to try PLU decomposition!");
    }

    const size_t n = matrix.row_count();
    std::vector<size_t> P(n);
    std::iota(P.begin(), P.end(), 0);

    Matrix<T> U = matrix;
    Matrix<T> L = identity_matrix<T>(n);

    for (size_t ib = 0; ib < n; ib += BLOCK_SIZE) {
        const size_t block_end = std::min(ib + BLOCK_SIZE, n);

        for (size_t i = ib; i < block_end && i < n - 1; ++i) {
            // Find pivot in current column
            size_t pivot_row = i;
            T max_val = std::abs(U.at(i, i));

            for (size_t j = i + 1; j < n; ++j) {
                T curr_val = std::abs(U.at(j, i));
                if (curr_val > max_val) {
                    max_val = curr_val;
                    pivot_row = j;
                }
            }

            // Swap rows
            if (pivot_row != i) {
                std::swap(P[i], P[pivot_row]);

                auto row_i = U.row_span(i);
                auto row_p = U.row_span(pivot_row);
                std::ranges::swap_ranges(row_i, row_p);

                if (i > 0) {
                    auto l_row_i = L.row_span(i).subspan(0, i);
                    auto l_row_p = L.row_span(pivot_row).subspan(0, i);
                    std::ranges::swap_ranges(l_row_i, l_row_p);
                }
            }

            T pivot = U.at(i, i);
            if (is_close(pivot, static_cast<T>(0), static_cast<T>(1e-9))) {
                continue;
            }

            const T inv_pivot = T(1) / pivot;

            for (size_t j = i + 1; j < n; ++j) {
                T mult = U.at(j, i) * inv_pivot;
                L.at(j, i) = mult;

#pragma omp simd
                for (size_t k = i + 1; k < block_end; ++k) {
                    U.at(j, k) -= mult * U.at(i, k);
                }
            }
        }

        if (block_end < n) {
#pragma omp parallel for schedule(static, 8) if (n - block_end > 128)
            for (size_t i = block_end; i < n; ++i) {
                for (size_t k = ib; k < block_end; ++k) {
                    T mult = L.at(i, k);
                    if (mult == T(0)) {
                        continue;
                    }

                    const T *pivot_row = &U.at(k, block_end);
                    T *target_row = &U.at(i, block_end);
                    const size_t len = n - block_end;

#pragma omp simd
                    for (size_t j = 0; j < len; ++j) {
                        target_row[j] -= mult * pivot_row[j];
                    }
                }
            }
        }
    }

    return std::make_tuple(std::move(P), std::move(L), std::move(U));
}

} // namespace maf::math

#endif
