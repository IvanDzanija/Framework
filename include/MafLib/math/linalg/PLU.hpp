#ifndef MATRIX_PLA_H
#define MATRIX_PLA_H

#pragma once
#include "Matrix.hpp"
namespace maf::math {

/// PLA decomposition of square matrix.
/// P - permutation, L - lower triangular, A - upper triangular
/// More information:
/// https://en.wikipedia.org/wiki/LA_decomposition#LA_factorization_with_partial_pivoting
/// P is saved as a std::vector of fixed size with the final row order!
/// @return Tuple of std::vector P, Matrix L, Matrix A.
template <typename T>
[[nodiscard]] std::tuple<std::vector<uint32>, Matrix<T>, Matrix<T>>
plu(const Matrix<T> &matrix) {
    if (!matrix.is_square()) {
        throw std::invalid_argument(
            "Matrix must be square to try PLU decomposition!");
    }

    const size_t n = matrix.row_count();
    std::vector<uint32> P(n);
    std::iota(P.begin(), P.end(), 0);

    Matrix<T> A = matrix;
    Matrix<T> L = identity_matrix<T>(n);

    for (size_t ib = 0; ib < n; ib += BLOCK_SIZE) {
        const size_t block_end = std::min(ib + BLOCK_SIZE, n);

        // Update blocks above current block
        if (ib >= BLOCK_SIZE) {
        }
        // #pragma omp parallel for
        for (size_t i = ib - BLOCK_SIZE; i < ib; ++i) {
            auto pivot_row = A.row_span(i).subspan(ib);
            for (size_t k = ib - BLOCK_SIZE; k < ib; ++k) {
                if (i >= k) {
                    continue;
                }
                T mult = L.at(k, i);
                if (mult == T(0)) {
                    continue;
                }
                auto target_row = A.row_span(k).subspan(ib);

                if (k + 1 < ib) {
                    __builtin_prefetch(&A.at(k + 1, ib), 0, 3);
                }
                const size_t len = n - ib;

#pragma omp simd
                for (size_t j = 0; j < len; ++j) {
                    target_row[j] -= mult * pivot_row[j];
                }
            }
        }

        // Process current block
        for (size_t i = ib; i < block_end && i < n - 1; ++i) {
            // Find pivot in current column
            size_t pivot_row = i;
            T max_val = std::abs(A.at(i, i));

            for (size_t j = i + 1; j < n; ++j) {
                T curr_val = std::abs(A.at(j, i));
                if (curr_val > max_val) {
                    max_val = curr_val;
                    pivot_row = j;
                }
            }

            // Swap rows
            if (pivot_row != i) {
                std::swap(P[i], P[pivot_row]);

                auto row_i = A.row_span(i);
                auto row_p = A.row_span(pivot_row);
                std::swap_ranges(row_i.begin(), row_i.end(), row_p.begin());

                if (i > 0) {
                    auto l_row_i = L.row_span(i).subspan(0, i);
                    auto l_row_p = L.row_span(pivot_row).subspan(0, i);
                    std::swap_ranges(l_row_i.begin(), l_row_i.end(),
                                     l_row_p.begin());
                }
            }

            T pivot = A.at(i, i);
            if (is_close(pivot, static_cast<T>(0), static_cast<T>(1e-9))) {
                std::cout << "singular " << std::endl;
                continue;
            }

            const T inv_pivot = T(1) / pivot;

            for (size_t j = i + 1; j < n; ++j) {
                T mult = A.at(j, i) * inv_pivot;
                L.at(j, i) = mult;

#pragma omp simd
                for (size_t k = i + 1; k < block_end; ++k) {
                    A.at(j, k) -= mult * A.at(i, k);
                }
            }
        }

        if (block_end < n) {
#pragma omp parallel for schedule(static, 8) if (n - block_end > 128)
            for (size_t i = block_end; i < n; ++i) {
                auto target_row = A.row_span(i).subspan(block_end);
                for (size_t k = ib; k < block_end; ++k) {
                    T mult = L.at(i, k);
                    if (mult == T(0)) {
                        continue;
                    }
                    auto pivot_row = A.row_span(k).subspan(block_end);

                    if (k + 1 < block_end) {
                        __builtin_prefetch(&A.at(k + 1, block_end), 0, 3);
                    }
                    const size_t len = n - block_end;

#pragma omp simd
                    for (size_t j = 0; j < len; ++j) {
                        target_row[j] -= mult * pivot_row[j];
                    }
                }
            }
        }
    }

    // Extract U
    Matrix<T> U(n, n);
#pragma omp parallel for schedule(static) if (n > 256)
    for (size_t i = 0; i < n; ++i) {
        T *u_row = &U.at(i, i);
        const T *a_row = &A.at(i, i);
        const size_t len = n - i;
        std::copy_n(a_row, len, u_row);
    }

    return std::make_tuple(std::move(P), std::move(L), std::move(U));
}

} // namespace maf::math

#endif
