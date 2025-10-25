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

    Matrix<T> A = matrix; // Work on a copy, we'll modify in place
    Matrix<T> L = identity_matrix<T>(n);

    for (size_t i = 0; i < n - 1; ++i) {
        // Find pivot
        size_t pivot_row = i;
        T max_val = std::abs(A.at(i, i));

        for (size_t j = i + 1; j < n; ++j) {
            T curr_val = std::abs(A.at(j, i));
            if (curr_val > max_val) {
                max_val = curr_val;
                pivot_row = j;
            }
        }

        // Actually swap rows in memory for cache efficiency
        if (pivot_row != i) {
            std::swap(P[i], P[pivot_row]);
            // Swap rows in A
            for (size_t k = 0; k < n; ++k) {
                std::swap(A.at(i, k), A.at(pivot_row, k));
            }
            // Swap already computed parts of L
            for (size_t k = 0; k < i; ++k) {
                std::swap(L.at(i, k), L.at(pivot_row, k));
            }
        }

        T pivot = A.at(i, i);
        if (is_close(pivot, static_cast<T>(0), static_cast<T>(1e-9))) {
            continue;
        }

        const T inv_pivot = T(1) / pivot; // Compute once

// Parallelize outer loop only for large matrices
#pragma omp parallel for schedule(static) if (n > 512)
        for (size_t j = i + 1; j < n; ++j) {
            T mult = A.at(j, i) * inv_pivot;
            L.at(j, i) = mult;

            // Vectorize inner loop, start from i (not 0!)
            auto pivot_span = A.row_span(i);
            auto target_span = A.row_span(j);
            T *t_ptr = target_span.data();
            const T *p_ptr = pivot_span.data();

#pragma omp simd
            for (size_t k = i; k < n; ++k) { // Start from i, not 0!
                t_ptr[k] -= mult * p_ptr[k];
            }
        }
    }

    // Extract U (upper triangular part of A)
    Matrix<T> U(n, n);
#pragma omp parallel for collapse(2) if (n > 256)
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i; j < n; ++j) {
            U.at(i, j) = A.at(i, j);
        }
    }

    return std::make_tuple(std::move(P), std::move(L), std::move(U));
}
