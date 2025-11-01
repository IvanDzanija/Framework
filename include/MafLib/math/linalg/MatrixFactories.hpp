#ifndef MATRIX_FACTORIES_H
#define MATRIX_FACTORIES_H
#pragma once
#include "Matrix.hpp"

/**
 * @file MatrixFactories.hpp
 * @brief Contains various implementations for producing specific matrices.
 *
 * This file is intended to be included at the *end* of Matrix.hpp and
 * should not be included directly anywhere else.
 */
namespace maf::math {
/**
 * @brief Creates a new identity matrix of a given size.
 * @tparam T The numeric type of the matrix.
 * @param size The width and height of the square matrix.
 * @return An identity Matrix<T> of size (size x size).
 */
template <typename T>
[[nodiscard]] Matrix<T> inline identity_matrix(size_t size) {
    Matrix<T> result(size, size);
    result.make_identity();
    return result;
}

/**
 * @brief Creates a new matrix filled with ones.
 * @tparam U The numeric type of the matrix.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return A Matrix<U> of size (rows x cols) filled with U(1).
 */
template <typename U>
[[nodiscard]] inline Matrix<U> ones(size_t rows, size_t cols) {
    Matrix<U> result(rows, cols);
    result.fill(U(1));
    return result;
}

/**
 * @brief Checks if two matrices are element-wise equal within a tolerance.
 * @tparam T Numeric type of the first matrix.
 * @tparam U Numeric type of the second matrix.
 * @param eps The absolute tolerance for equality.
 * @return true if dimensions match and all elements are "close".
 */
template <typename T, typename U>
[[nodiscard]] constexpr bool loosely_equal(const Matrix<T> &first,
                                           const Matrix<U> &second,
                                           double eps = 1e-6) {
    size_t n = first.row_count();
    size_t m = first.column_count();
    if (n != second.row_count() || m != second.column_count()) {
        return false;
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (!is_close(first.at(i, j), second.at(i, j), eps)) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Creates a new permutation matrix from a permutation vector.
 * @tparam T The numeric type of the matrix.
 * @param perm A vector where `perm[i] = j` means row `i` maps to
 * column `j`.
 * @return A sparse permutation Matrix<T>.
 */
template <typename T>
[[nodiscard]] Matrix<T> inline make_permutation_matrix(
    const std::vector<uint32> &perm) {
    size_t n = perm.size();
    Matrix<T> result(n, n); // Initializes to zero
#pragma omp parallel for if (n > 256)
    for (size_t i = 0; i < n; ++i) {
        const size_t j = perm.at(i);
        result.at(i, j) = static_cast<T>(1);
    }

    return result;
}

} // namespace maf::math

#endif
