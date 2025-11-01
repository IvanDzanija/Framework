#ifndef MATRIX_PLU_H
#define MATRIX_PLU_H

#pragma once
#include "Matrix.hpp"

namespace maf::math {
/**
 * @brief Performs a blocked PLU decomposition on a square matrix.
 *
 * This function computes the PLU factorization (PA = LU) for a given square
 * matrix A, where:
 * - P is a permutation matrix (represented by a vector)
 * - L is a unit lower triangular matrix
 * - U is an upper triangular matrix
 *
 * The decomposition is computed using a right-looking blocked algorithm for
 * improved cache performance. It employs partial pivoting (row swapping) to
 * ensure numerical stability. The implementation is parallelized using OpenMP
 * for further performance gains on multi-core systems.
 *
 * More information:
 * https://en.wikipedia.org/wiki/LU_decomposition#LU_factorization_with_partial_pivoting
 *
 * @param matrix The const reference to the square input matrix (A) to
 * decompose.
 * @return A std::tuple containing:
 * 1. (std::vector<uint32>) The final permutaion of rows.
 * 2. (Matrix<T>) The unit lower triangular matrix (L).
 * 3. (Matrix<T>) The upper triangular matrix (U).
 *
 * @throws std::invalid_argument if the input matrix is not square.
 *
 * @version 1.0 (Blocked & Parallelized)
 * @since 2025
 */
template <typename T>
[[nodiscard]] std::tuple<std::vector<uint32>, Matrix<T>, Matrix<T>>
plu(const Matrix<T> &matrix) {
    if (!matrix.is_square()) {
        throw std::invalid_argument(
            "Matrix must be square for PLU decomposition!");
    }

    const size_t n = matrix.row_count();
    if (n == 0) {
        return {std::vector<uint32>(), Matrix<T>(), Matrix<T>()};
    }

    std::vector<uint32> P(n);
    // Change this to ranges::iota when Apple Clang fully supports c++23
    std::iota(P.begin(), P.end(), 0);
    Matrix<T> _U = matrix;
    Matrix<T> L = identity_matrix<T>(n);

    // Conceptual block matrix:
    // A = [A_11, A_12]
    //     [A_21, A_22]

    for (size_t ib = 0; ib < n; ib += BLOCK_SIZE) {
        const size_t block_end = std::min(ib + BLOCK_SIZE, n);
        // Panel Factorization
        // This computes L_11, L_21, U_11 and updates P
        for (size_t i = ib; i < block_end && i < n - 1; ++i) {
            // Find Pivot
            size_t pivot_row = i;
            T max_val = std::abs(_U.at(i, i));

            for (size_t j = i + 1; j < n; ++j) {
                T curr_val = std::abs(_U.at(j, i));
                if (curr_val > max_val) {
                    max_val = curr_val;
                    pivot_row = j;
                }
            }

            if (is_close(max_val, static_cast<T>(0), 1e-9)) {
                throw std::runtime_error(
                    "Matrix is singular; pivot is near zero.");
            }

            if (pivot_row != i) {
                // Swap permutation vector
                std::swap(P[i], P[pivot_row]);

                // Swap entire rows in our working matrix
                auto row_i = _U.row_span(i);
                auto row_p = _U.row_span(pivot_row);
                std::ranges::swap_ranges(row_i, row_p);

                // Swap the already computed part of L
                if (i > 0) {
                    auto l_row_i = L.row_span(i).subspan(0, i);
                    auto l_row_p = L.row_span(pivot_row).subspan(0, i);
                    std::ranges::swap_ranges(l_row_i, l_row_p);
                }
            }

            // Elimination within panel
            const T pivot = _U.at(i, i);
            const T inv_pivot = T(1) / pivot;

#pragma omp parallel for schedule(static) if (n - (i + 1) > 256)
            for (size_t j = i + 1; j < n; ++j) {
                T mult = _U.at(j, i) * inv_pivot;
                L.at(j, i) = mult;

#pragma omp simd
                for (size_t k = i + 1; k < block_end; ++k) {
                    _U.at(j, k) -= mult * _U.at(i, k);
                }
            }
        }

        // Update Trailing Matrix
        if (block_end < n) {
            // Triangular Solve for U_12
            // We must solve L_11 * U_12 = A_12
#pragma omp parallel for schedule(static, 8) if (n - block_end > 128)
            for (size_t j = block_end; j < n; ++j) {
                for (size_t i = ib; i < block_end; ++i) {
                    T sum = _U.at(i, j);

                    // Can't be SIMD because of column fetching
                    for (size_t k = ib; k < i; ++k) {
                        sum -= L.at(i, k) * _U.at(k, j);
                    }
                    _U.at(i, j) = sum;
                }
            }

            // Now we can apply all the elimination effects on the next block
#pragma omp parallel for schedule(static, 8) if (n - block_end > 128)
            for (size_t i = block_end; i < n; ++i) {
                auto target_row = _U.row_span(i).subspan(block_end);
                const size_t len = target_row.size();
                for (size_t k = ib; k < block_end; ++k) {
                    const T mult = L.at(i, k);
                    if (is_close(mult, static_cast<T>(0),
                                 static_cast<T>(1e-9))) {
                        continue;
                    }
                    auto pivot_row = _U.row_span(k).subspan(block_end);

#pragma omp simd
                    for (size_t j = 0; j < len; ++j) {
                        target_row[j] -= mult * pivot_row[j];
                    }
                }
            }
        }
    }
    if (is_close(_U.at(n - 1, n - 1), static_cast<T>(0), 1e-9)) {
        throw std::runtime_error("Matrix is singular; pivot is near zero.");
    }

    // Extract U
    Matrix<T> U(n, n);
#pragma omp parallel for schedule(static) if (n > 256)
    for (size_t i = 0; i < n; ++i) {
        T *u_row = &U.at(i, i);
        const T *a_row = &_U.at(i, i);
        const size_t len = n - i;
        std::copy_n(a_row, len, u_row);
    }

    return std::make_tuple(std::move(P), std::move(L), std::move(U));
}

} // namespace maf::math

#endif // MATRIX_PLU_H
