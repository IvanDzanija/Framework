#ifndef MATRIX_H
#define MATRIX_H
#pragma once
#include "MafLib/main/GlobalHeader.hpp"

namespace maf::math {
template <typename T> class Vector;

/**
 * @brief A general-purpose, row-major, dense matrix class.
 *
 * This class implements a matrix with contiguous row-major storage.
 * It supports a wide range of arithmetic operations, constructors, and
 * utility methods.
 *
 * The implementation is templated to support various numeric types (T).
 * Many operations (like multiplication and addition) are parallelized
 * using OpenMP and employ blocking strategies for cache efficiency.
 *
 * @tparam T The numeric type of the matrix elements (e.g., float,
 * double, int).
 *
 * @version 1.0
 * @since 2025
 */
template <typename T> class Matrix {
  private:
    size_t _rows;
    size_t _cols;
    std::vector<T> _data;

    /**
     * @brief Internal check if a row/column index is within bounds.
     * @return true if 0 <= row < _rows and 0 <= col < _cols.
     */
    [[nodiscard]] constexpr bool _is_valid_index(size_t row, size_t col) const {
        return row < _rows && col < _cols;
    }

    /**
     * @brief Converts a 2D (row, col) index to a 1D internal vector index.
     * @throws std::out_of_range if the index is invalid.
     * @return The 1D index into the _data vector.
     */
    [[nodiscard]] constexpr size_t _get_index(size_t row, size_t col) const {
        if (!_is_valid_index(row, col)) {
            throw std::out_of_range("Index out of bounds.");
        }
        return (row * _cols) + col;
    }

    /**
     * @brief Internal helper to invert the sign of all elements in-place.
     */
    void _invert_sign() {
#pragma omp parallel for
        for (size_t i = 0; i < _data.size(); ++i) {
            _data[i] = -_data[i];
        }
    }

  public:
    // --- Constructors and destructors ---

    /**
     * @brief Default constructor. Creates an empty 0x0 matrix.
     */
    Matrix() : _rows(0), _cols(0) {}

    /**
     * @brief Constructs an uninitialized matrix of size rows x cols.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @throws std::invalid_argument if dimensions are zero.
     */
    Matrix(size_t rows, size_t cols);

    /**
     * @brief Constructs a matrix from a raw data pointer.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param data Pointer to a C-style array of size (rows * cols) in
     * row-major order. The data is COPIED into the matrix.
     * @throws std::invalid_argument if dimensions are zero or data is
     * nullptr.
     */
    Matrix(size_t rows, size_t cols, T *data);

    /**
     * @brief Constructs from a std::vector, filled by rows.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param data A std::vector of size (rows * cols) in row-major order.
     * @throws std::invalid_argument if dimensions are zero or data size
     * does not match.
     */
    Matrix(size_t rows, size_t cols, const std::vector<T> &data);

    /**
     * @brief Constructs from a nested std::vector (vector of vectors).
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param data A std::vector<std::vector<T>>. data.size() must equal
     * rows, and data[0].size() must equal cols.
     * @throws std::invalid_argument if dimensions are zero or data shape
     * does not match.
     */
    Matrix(size_t rows, size_t cols, const std::vector<std::vector<T>> &data);

    /**
     * @brief Constructs from a std::array, filled by rows.
     * @tparam U Type in the array (allows implicit conversion).
     * @tparam N Size of the array.
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param data A std::array<U, N> where N must equal (rows * cols).
     * @throws std::invalid_argument if dimensions are zero or array size
     * does not match.
     */
    template <typename U, size_t N>
    Matrix(size_t rows, size_t cols, const std::array<U, N> &data);

    /**
     * @brief Constructs from a std::initializer_list, filled by rows.
     * @tparam U Type in the list (allows implicit conversion).
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param list A std::initializer_list<U> of size (rows * cols).
     * @throws std::invalid_argument if dimensions are zero or list size
     * does not match.
     */
    template <typename U>
    Matrix(size_t rows, size_t cols, std::initializer_list<U> list);

    // --- Getters and setters ---

    /**
     * @brief Gets a mutable reference to the underlying std::vector
     * data store.
     * @return std::vector<T>&
     */
    [[nodiscard]] std::vector<T> &data() noexcept { return _data; }

    /**
     * @brief Gets a const reference to the underlying std::vector data
     * store.
     * @return const std::vector<T>&
     */
    [[nodiscard]] const std::vector<T> &data() const noexcept { return _data; }

    /** @brief Gets the number of rows. */
    [[nodiscard]] size_t row_count() const noexcept { return _rows; }

    /** @brief Gets the number of columns. */
    [[nodiscard]] size_t column_count() const noexcept { return _cols; }

    /** @brief Gets the total number of elements (rows * cols). */
    [[nodiscard]] size_t size() const noexcept { return _data.size(); }

    /**
     * @brief Gets a mutable reference to the element at (row, col).
     * @throws std::out_of_range if the index is invalid.
     */
    T &at(size_t row, size_t col) { return _data.at(_get_index(row, col)); }

    /**
     * @brief Gets a const reference to the element at (row, col).
     * @throws std::out_of_range if the index is invalid.
     */
    const T &at(size_t row, size_t col) const {
        return _data.at(_get_index(row, col));
    }

    /**
     * @brief Gets a mutable std::span of a single row.
     * @throws std::out_of_range if the row is invalid.
     */
    [[nodiscard]] std::span<T> row_span(size_t row) {
        return std::span<T>(&_data.at(_get_index(row, 0)), _cols);
    }

    /**
     * @brief Gets a const std::span of a single row.
     * @throws std::out_of_range if the row is invalid.
     */
    [[nodiscard]] std::span<const T> row_span(size_t row) const {
        return std::span<const T>(&_data.at(_get_index(row, 0)), _cols);
    }

    // --- Checkers ---

    /** @brief Checks if the matrix is square (rows == cols). */
    [[nodiscard]] constexpr bool is_square() const;

    /** @brief Checks if the matrix is symmetric (A == A^T). */
    [[nodiscard]] constexpr bool is_symmetric() const;

    /** @brief Checks if the matrix is upper triangular */
    [[nodiscard]] constexpr bool is_upper_triangular() const;

    /** @brief Checks if the matrix is lower triangular */
    [[nodiscard]] constexpr bool is_lower_triangular() const;

    /** @brief Checks if the matrix is diagonal */
    [[nodiscard]] constexpr bool is_diagonal() const;

    /**
     * @brief Checks if the matrix is positive definite.
     * @details For symmetric matrices, attempts Cholesky decomposition.
     * TODO: Add Sylvester's criterion for non-symmetric.
     * Defined in MatrixCheckers.hpp
     */
    [[nodiscard]] bool is_positive_definite() const;

    /**
     * @brief Checks if the matrix is singular (non-invertible).
     * @details Equivalent to det(A) == 0.
     * @details Equivalent to rank(A) < max(dimensions(A)).
     */
    [[nodiscard]] constexpr bool is_singular() const;

    // --- Methods ---

    /** @brief Fills the entire matrix with a single value.*/
    void fill(T value);

    /**
     * @brief Converts this matrix into an identity matrix.
     * @throws std::runtime_error if the matrix is not square.
     * @details Defined in MatrixMethods.hpp
     */
    void make_identity();

    /**
     * @brief Performs an in-place transpose of the matrix.
     * @details Uses a parallelized, blocked algorithm.
     * @throws std::invalid_argument if the matrix is not square.
     */
    void transpose();

    /**
     * @brief Creates and returns a new matrix that is the transpose of
     * this one.
     * @return A new Matrix<T> of size (cols x rows).
     * @details Defined in MatrixMethods.hpp
     */
    [[nodiscard]] Matrix<T> transposed() const;

    // --- Operators ---

    /**
     * @brief Checks for exact element-wise equality.
     * @details For floating-point, use `loosely_equal()`.
     */
    [[nodiscard]] constexpr bool operator==(const Matrix &other) const {
        if (_rows != other._rows || _cols != other._cols) {
            return false;
        }
        return _data == other._data;
    }

    /**
     * @brief Unary minus. Returns a new matrix with all elements negated.
     * @return `Matrix<T>`
     */
    [[nodiscard]] auto operator-() const {
        Matrix<T> result = *this;
        result._invert_sign();
        return result;
    }

    /**
     * @brief Element-wise matrix addition.
     * @tparam U Numeric type of the other matrix.
     * @return Matrix of the common, promoted type.
     * @throws std::invalid_argument if dimensions do not match.
     */
    template <typename U>
    [[nodiscard]] auto operator+(const Matrix<U> &other) const {
        using R = std::common_type_t<T, U>;

        if (_rows != other.row_count() || _cols != other.column_count()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions for addition!");
        }
        Matrix<R> result(_rows, _cols);

        if (_data.size() < 100 * 100) {
#pragma omp parallel for collapse(2)
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result.at(i, j) = static_cast<R>(this->at(i, j)) +
                                      static_cast<R>(other.at(i, j));
                }
            }
        } else {
#pragma omp parallel for schedule(static, 1024)
            for (size_t idx = 0; idx < _data.size(); ++idx) {
                result.data()[idx] = static_cast<R>(_data[idx]) +
                                     static_cast<R>(other.data()[idx]);
            }
        }
        return result;
    }

    /**
     * @brief Element-wise scalar addition (Matrix + scalar).
     * @tparam U An arithmetic scalar type.
     * @return Matrix of the common, promoted type.
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] auto operator+(const U &scalar) const {
        using R = std::common_type_t<T, U>;
        Matrix<R> result(_rows, _cols);
        std::transform(_data.begin(), _data.end(), result.data().begin(),
                       [scalar](const T &value) {
                           return static_cast<R>(value) +
                                  static_cast<R>(scalar);
                       });
        return result;
    }

    /**
     * @brief Element-wise scalar addition (scalar + Matrix).
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] friend auto operator+(const U &scalar,
                                        const Matrix<T> &matrix) {
        return matrix + scalar;
    }

    /**
     * @brief Element-wise matrix subtraction.
     * @tparam U Numeric type of the other matrix.
     * @return Matrix of the common, promoted type.
     * @throws std::invalid_argument if dimensions do not match.
     */
    template <typename U>
    [[nodiscard]] auto operator-(const Matrix<U> &other) const {
        if (_rows != other.row_count() || _cols != other.column_count()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions for subtraction!");
        }

        using R = std::common_type_t<T, U>;
        Matrix<R> result(_rows, _cols);

        if (_data.size() < 100 * 100) {
#pragma omp parallel for collapse(2)
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result.at(i, j) = static_cast<R>(this->at(i, j)) -
                                      static_cast<R>(other.at(i, j));
                }
            }
        } else {
#pragma omp parallel for schedule(static, 1024)
            for (size_t idx = 0; idx < _data.size(); ++idx) {
                result.data()[idx] = static_cast<R>(_data[idx]) -
                                     static_cast<R>(other.data()[idx]);
            }
        }
        return result;
    }

    /**
     * @brief Element-wise scalar subtraction (Matrix - scalar).
     * @tparam U An arithmetic scalar type.
     * @return Matrix of the common, promoted type.
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] auto operator-(const U &scalar) const {
        using R = std::common_type_t<T, U>;
        Matrix<R> result(_rows, _cols);
        std::transform(_data.begin(), _data.end(), result.data().begin(),
                       [scalar](const T &value) {
                           return static_cast<R>(value) -
                                  static_cast<R>(scalar);
                       });
        return result;
    }

    /**
     * @brief Element-wise scalar subtraction (scalar - Matrix).
     * @tparam U An arithmetic scalar type.
     * @return Matrix of the common, promoted type.
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] friend auto operator-(const U &scalar,
                                        const Matrix<T> &matrix) {
        using R = std::common_type_t<T, U>;

        Matrix<R> result(matrix._rows, matrix._cols);
        std::transform(matrix._data.begin(), matrix._data.end(),
                       result.data().begin(), [scalar](const T &value) {
                           return static_cast<R>(scalar) -
                                  static_cast<R>(value);
                       });
        return result;
    }

    /**
     * @brief Standard algebraic matrix multiplication (A * B).
     * @details Implemented with a parallelized, cache-blocked algorithm.
     * @tparam U Numeric type of the other matrix.
     * @return Matrix of the common, promoted type.
     * @throws std::invalid_argument if inner dimensions do not match
     * (A.cols != B.rows).
     */
    template <typename U>
    [[nodiscard]] auto operator*(const Matrix<U> &other) const {
        if (_cols != other.row_count()) {
            throw std::invalid_argument(
                "Matrix inner dimensions do not match for multiplication!");
        }
        using R = std::common_type_t<T, U>;

        const size_t a_rows = _rows;
        const size_t b_cols = other.column_count();
        const size_t a_cols = _cols;
        Matrix<R> result(a_rows, b_cols);

        const T *a_data = this->_data.data();
        const U *b_data = other.data().data();
        R *c_data = result.data().data();

        result.fill(R(0));

#pragma omp parallel for collapse(2) if (a_rows * b_cols > 10000)
        for (size_t ii = 0; ii < a_rows; ii += BLOCK_SIZE) {
            for (size_t jj = 0; jj < b_cols; jj += BLOCK_SIZE) {
                for (size_t kk = 0; kk < a_cols; kk += BLOCK_SIZE) {
                    // Process block
                    const size_t i_end = std::min(ii + BLOCK_SIZE, a_rows);
                    const size_t j_end = std::min(jj + BLOCK_SIZE, b_cols);
                    const size_t k_end = std::min(kk + BLOCK_SIZE, a_cols);

                    for (size_t i = ii; i < i_end; ++i) {
                        for (size_t k = kk; k < k_end; ++k) {
                            const R a_ik =
                                static_cast<R>(a_data[(i * a_cols) + k]);
                            const size_t b_offset = k * b_cols;
                            const size_t c_offset = i * b_cols;

#pragma omp simd
                            for (size_t j = jj; j < j_end; ++j) {
                                c_data[c_offset + j] +=
                                    a_ik * static_cast<R>(b_data[b_offset + j]);
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

    /**
     * @brief Element-wise scalar multiplication (Matrix * scalar).
     * @tparam U An arithmetic scalar type.
     * @return Matrix of the common, promoted type.
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] auto operator*(const U &scalar) const {
        using R = std::common_type_t<T, U>;
        Matrix<R> result(_rows, _cols);

        std::transform(_data.begin(), _data.end(), result.data().begin(),
                       [scalar](const T &value) {
                           return static_cast<R>(value) *
                                  static_cast<R>(scalar);
                       });
        return result;
    }

    /**
     * @brief Element-wise scalar multiplication (scalar * Matrix).
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] friend auto operator*(const U &scalar,
                                        const Matrix<T> &matrix) {
        return matrix * scalar;
    }

    /**
     * @brief Matrix-Vector multiplication (Matrix * column_vector).
     * @tparam U Numeric type of the vector.
     * @return A new column Vector of the common, promoted type.
     * @throws std::invalid_argument if vector is not a column vector or
     * dimensions do not match.
     */
    template <typename U>
    [[nodiscard]] auto operator*(const Vector<U> &other) const {
        using R = std::common_type_t<T, U>;

        const size_t n = this->row_count();
        const size_t m = this->column_count();

        if (other.orientation() == Vector<U>::ROW) {
            throw std::invalid_argument(
                "Invalid multiplication: matrix * row vector.\n"
                "Did you mean Vector * Matrix?");
        }

        if (other.size() != m) {
            throw std::invalid_argument(
                "Dimension mismatch in Matrix * Vector multiplication.");
        }

        Vector<R> result(n, std::vector<R>(n, R(0)), Vector<R>::COLUMN);

#pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            R sum = R(0);
            auto L_row_i = this->row_span(i);
#pragma omp simd reduction(+ : sum)
            for (size_t j = 0; j < m; ++j) {
                sum += static_cast<R>(L_row_i[j]) * static_cast<R>(other.at(j));
            }
            result.at(i) = sum;
        }
        return result;
    }

    /**
     * @brief Element-wise scalar division (Matrix / scalar).
     * @tparam U An arithmetic scalar type.
     * @return Matrix of the common, promoted type.
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] auto operator/(const U &scalar) const {
        // Note: This promotes T to double if T is int, which is
        // usually desired for division.
        using R = std::common_type_t<T, U, double>;
        return *this * (R(1) / static_cast<R>(scalar));
    }

    /**
     * @brief Element-wise scalar division (scalar / Matrix).
     * @tparam U An arithmetic scalar type.
     * @return Matrix of the common, promoted type.
     */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] friend auto operator/(const U &scalar,
                                        const Matrix<T> &matrix) {
        using R = std::common_type_t<T, U, double>;

        Matrix<R> result(matrix._rows, matrix._cols);
        std::transform(matrix._data.begin(), matrix._data.end(),
                       result.data().begin(), [scalar](const T &value) {
                           return static_cast<R>(scalar) /
                                  static_cast<R>(value);
                       });
        return result;
    }

    /** @brief In-place element-wise scalar addition. */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    auto &operator+=(const U &scalar) {
        std::transform(
            _data.begin(), _data.end(), _data.begin(),
            [scalar](T val) { return static_cast<T>(val + scalar); });
        return *this;
    }

    /** @brief In-place element-wise scalar subtraction. */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    auto &operator-=(const U &scalar) {
        std::transform(
            _data.begin(), _data.end(), _data.begin(),
            [scalar](T val) { return static_cast<T>(val - scalar); });
        return *this;
    }

    /** @brief In-place element-wise scalar multiplication. */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    auto &operator*=(const U &scalar) {
        std::transform(
            _data.begin(), _data.end(), _data.begin(),
            [scalar](T val) { return static_cast<T>(val * scalar); });
        return *this;
    }

    /** @brief In-place element-wise scalar division. */
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    auto &operator/=(const U &scalar) {
        std::transform(
            _data.begin(), _data.end(), _data.begin(),
            [scalar](T val) { return static_cast<T>(val / scalar); });
        return *this;
    }

    // --- Debugging and printing ---

    /**
     * @brief Prints the matrix contents to std::cout.
     * @details Sets floating point precision for readability.
     */
    void print() const {
        if constexpr (std::is_floating_point_v<T>) {
            std::cout << std::fixed << std::setprecision(5);
        }
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                std::cout << this->at(i, j) << ' ';
            }
            std::cout << std::endl;
        }
    }
};

} // namespace maf::math

#include "CholeskyDecomposition.hpp"
#include "MatrixCheckers.hpp"
#include "MatrixConstructors.hpp"
#include "MatrixFactories.hpp"
#include "MatrixMethods.hpp"
#include "PLU.hpp"

#endif
