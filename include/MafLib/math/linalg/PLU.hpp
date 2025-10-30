#ifndef MATRIX_PLA_H
#define MATRIX_PLA_H

#include "MafLib/main/GlobalHeader.hpp"
#pragma once
#include "Matrix.hpp"
namespace maf::math {

/// PLU decomposition of square matrix.
/// P - permutation, L - lower triangular, U - upper triangular
/// More information:
/// https://en.wikipedia.org/wiki/LA_decomposition#LA_factorization_with_partial_pivoting
/// P is saved as a std::vector of fixed size with the final row order!
/// @return Tuple of std::vector P, Matrix L, Matrix U.
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

    for (size_t i = 0; i < n - 1; ++i) {
        size_t pivot_row = i;
        T max_val = std::abs(A.at(i, i));

        for (size_t j = i + 1; j < n; ++j) {
            T curr_val = std::abs(A.at(j, i));
            if (curr_val > max_val) {
                max_val = curr_val;
                pivot_row = j;
            }
        }

        if (is_close(max_val, static_cast<T>(0), static_cast<T>(1e-9))) {
            continue;
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
                std::ranges::swap_ranges(l_row_i, l_row_p);
            }
        }

        T pivot = A.at(i, i);
        const T inv_pivot = T(1) / pivot;

#pragma omp parallel for schedule(static) if (n - i > BLOCK_SIZE)
        for (size_t j = i + 1; j < n; ++j) {
            T mult = A.at(j, i) * inv_pivot;
            L.at(j, i) = mult;

#pragma omp simd
            for (size_t k = i + 1; k < n; ++k) {
                A.at(j, k) -= mult * A.at(i, k);
            }
        }
    }

    Matrix<T> U(n, n);
#pragma omp parallel for schedule(static) if (n > 256)
    for (size_t i = 0; i < n; ++i) {
        auto u_row = U.row_span(i).subspan(i);
        auto a_row = A.row_span(i).subspan(i);
        std::ranges::copy(a_row, u_row.begin());
    }

    return std::make_tuple(std::move(P), std::move(L), std::move(U));
}

} // namespace maf::math

#endif
