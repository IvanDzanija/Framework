#ifndef MATRIX_PLU_H
#define MATRIX_PLU_H

#pragma once
#include "Matrix.hpp"
namespace maf {

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

    // Every square matrix has PLU decomposition.
    // If matrix is not of full rank we can use this to calculate the rank of
    // input matrix by counting the zeros on the diagonal of U.
    // Obviously following this thinking we can calculate the determinant of
    // matrix by using this or Cholesky decomposition

    const size_t n = matrix.row_count();
    std::vector<size_t> P(n);
    std::iota(P.begin(), P.end(), 0); // Change this when Apple clang fully
    // supports c++23
    Matrix<T> L(n, n);
    Matrix<T> U = matrix;

    for (size_t i = 0; i < n - 1; ++i) {
        // Find the pivot
        T pivot = std::abs(U.at(P.at(i), i));
        size_t pivot_row = i;
        for (size_t j = i + 1; j < n; ++j) {
            T curr_val = std::abs(U.at(P.at(j), i));
            if (pivot < curr_val) {
                pivot = curr_val;
                pivot_row = j;
            }
        }
        // Indirectly swap the rows
        std::swap(P.at(i), P.at(pivot_row));
        pivot = U.at(P.at(i), i);

        if (is_close(pivot, static_cast<T>(0), static_cast<T>(1e-9))) {
            continue;
        }

#pragma omp parallel for schedule(dynamic) if (n - i > 64)
        for (size_t j = i + 1; j < n; ++j) {
            T mult = U.at(P[j], i) / pivot;
            L.at(P[j], i) = mult;

            auto pivot_span = U.row_span(P.at(i));
            auto target_span = U.row_span(P.at(j));
            T *t_ptr = target_span.data();
            const T *p_ptr = pivot_span.data();

#pragma omp simd
            for (size_t k = 0; k < n; ++k) {
                t_ptr[k] -= mult * p_ptr[k];
            }
        }
    }

    Matrix<T> L_final = identity_matrix<T>(n);
    Matrix<T> U_final(n, n);

    // Reconstruct U
#pragma omp parallel for if (n > 100)
    for (size_t i = 0; i < n; ++i) {
        auto U_perm_span = U.row_span(P[i]);
        auto U_final_span = U_final.row_span(i);
        const T *U_src = U_perm_span.data();
        T *U_dst = U_final_span.data();

#pragma omp simd
        for (size_t j = i; j < n; ++j) {
            U_dst[j] = U_src[j];
        }
    }

    // Reconstruct L
#pragma omp parallel for if (n > 100)
    for (size_t i = 0; i < n; ++i) {
        auto L_perm_span = L.row_span(P[i]);
        auto L_final_span = L_final.row_span(i);

        const T *L_src = L_perm_span.data();
        T *L_dst = L_final_span.data();

#pragma omp simd
        for (size_t j = 0; j < i; ++j) {
            L_dst[j] = L_src[j];
        }
    }

    return std::make_tuple(std::move(P), std::move(L_final),
                           std::move(U_final));
}

} // namespace maf

#endif
