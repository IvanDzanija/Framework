#ifndef MATRIX_H
#define MATRIX_H
#pragma once
#include "MafLib/main/GlobalHeader.hpp"

#define BLOCK_SIZE 64

namespace maf {
template <typename T> class Vector;

template <typename T> class Matrix {
  private:
    size_t _rows;
    size_t _cols;
    std::vector<T> _data;

    [[nodiscard]] constexpr bool _is_valid_index(size_t row, size_t col) const {
        return row < _rows && col < _cols;
    }

    [[nodiscard]] constexpr size_t _get_index(size_t row, size_t col) const {
        if (!_is_valid_index(row, col)) {
            throw std::out_of_range("Index out of bounds.");
        }
        return (row * _cols) + col;
    }

    void _invert_sign() {
        for (auto &element : _data) {
            element = -element;
        }
    }

  public:
    // Constructors and destructors

    // Default constructor
    Matrix() : _rows(0), _cols(0) {}

    // Construct from raw data
    Matrix(size_t rows, size_t cols, T *data) : _rows(rows), _cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero!");
        }
        if (data == nullptr) {
            throw std::invalid_argument("Data pointer cannot be null!");
        }

        _data.assign(data, data + (rows * cols));
    }

    // Construct empty of size rows x cols
    Matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }

        _data.resize(rows * cols);
    }

    // Constructors for std::vector and std::array

    /// Constructor from std::vector
    /// Matrix gets filled by rows.
    Matrix(size_t rows, size_t cols, const std::vector<T> &data)
        : _rows(rows), _cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }

        if (data.size() != rows * cols) {
            throw std::invalid_argument(
                "Data size does not match matrix size.");
        }

        _data.assign(data.begin(), data.end());
    }

    /// Matrix constructor from std::vector<std::vector>>
    /// Matrix indices correspond to indices in std::vector<std::vector>>
    Matrix(size_t rows, size_t cols, const std::vector<std::vector<T>> &data)
        : _rows(rows), _cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }

        if (data.size() != rows || data.at(0).size() != cols) {
            throw std::invalid_argument(
                "Data size does not match matrix size.");
        }

        _data.resize(rows * cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                _data.at(_get_index(i, j)) = static_cast<T>(data.at(i).at(j));
            }
        }
    }

    /// Matrix constructor from std::array
    /// Matrix gets filled by rows.
    template <typename U, size_t N>
    Matrix(size_t rows, size_t cols, const std::array<U, N> &data)
        : _rows(rows), _cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }
        if (N != rows * cols) {
            throw std::invalid_argument(
                "Data size does not match matrix size.");
        }

        _data.assign(data.begin(), data.end());
    }

    /// Matrix constructor from std::initializer_list
    /// Matrix gets filled by rows.
    template <typename U>
    Matrix(size_t rows, size_t cols, std::initializer_list<U> list)
        : _rows(rows), _cols(cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }
        if (static_cast<size_t>(list.size()) != rows * cols) {
            throw std::invalid_argument(
                "Data size does not match matrix size.");
        }
        _data.assign(list.begin(), list.end());
    }

    // Getters and setters

    [[nodiscard]] std::vector<T> &data() noexcept { return _data; }
    [[nodiscard]] const std::vector<T> &data() const noexcept { return _data; }
    [[nodiscard]] size_t row_count() const noexcept { return _rows; }
    [[nodiscard]] size_t column_count() const noexcept { return _cols; }
    [[nodiscard]] size_t size() const noexcept { return _data.size(); }

    T &at(size_t row, size_t col) { return _data.at(_get_index(row, col)); }

    const T &at(size_t row, size_t col) const {
        return _data.at(_get_index(row, col));
    }

    [[nodiscard]] std::span<T> row_span(size_t row) {
        return std::span<T>(&_data.at(_get_index(row, 0)), _cols);
    }

    [[nodiscard]] std::span<const T> row_span(size_t row) const {
        return std::span<const T>(&_data.at(_get_index(row, 0)), _cols);
    }

    // Checkers
    /// Checks if the matrix is square.
    [[nodiscard]] constexpr bool is_square() const { return _rows == _cols; }

    /// Checkes if the matrix is symmetric.
    [[nodiscard]] constexpr bool is_symmetric() const;

    /// Checks if the matrix is upper triangular.
    [[nodiscard]] constexpr bool is_upper_triangular() const;

    /// Checks if the matrix is lower triangular.
    [[nodiscard]] constexpr bool is_lower_triangular() const;

    /// Checks if the matrix is diagonal.
    [[nodiscard]] constexpr bool is_diagonal() const;

    /// Checks if matrix is positive definite.
    /// If matrix is symmetric it is sufficient to have Cholesky decomposition.
    /// TODO: If matrix isn't symmetric
    [[nodiscard]] bool is_positive_definite() const;

    /// Checks if the matrix is singular.
    /// This is equivalent to:
    /// 1) Matrix A doesn't have an inverse.
    /// 2) Rank (A) < n
    /// 3) Determinant(A) = 0
    /// 4) Eigen value(A) = 0
    [[nodiscard]] constexpr bool is_singular() const;

    // Methods
    /// Fills the current matrix with $value.
    void fill(T value);

    /// Converts the matrix to indentity matrix.
    void make_identity();

    /// Inplace transpose of matrix for square matrices only.
    /// For transposing non square matrices use .transposed() that creates a new
    /// Matrix object and returns it.
    void transpose() {
        if (!is_square()) {
            throw std::invalid_argument("Matrix must be square to transpose.");
        }

#pragma omp parallel for schedule(dynamic) collapse(2)
        for (size_t i = 0; i < _rows; i += BLOCK_SIZE) {
            for (size_t j = i; j < _cols; j += BLOCK_SIZE) {
                const size_t n = i + BLOCK_SIZE;
                const size_t m = j + BLOCK_SIZE;
                if (i == j) {
                    for (size_t k = i; k < std::min(n, _rows); ++k) {
                        for (size_t l = k + 1; l < std::min(m, _cols); ++l) {
                            std::swap(this->at(k, l), this->at(l, k));
                        }
                    }
                } else {
                    for (size_t k = i; k < std::min(n, _rows); ++k) {
                        for (size_t l = j; l < std::min(m, _cols); ++l) {
                            std::swap(this->at(k, l), this->at(l, k));
                        }
                    }
                }
            }
        }
    }

    /// Creates new transposed matrix
    /// @return Transposed matrix
    [[nodiscard]] Matrix<T> transposed() const;

    // Operators

    // Matrix == Matrix
    /// Checks if 2 matrices are exactly same,
    /// use loosely_equal if working with floats.
    [[nodiscard]] constexpr bool operator==(const Matrix &other) const {
        if (_rows != other._rows || _cols != other._cols) {
            return false;
        }
        return _data == other._data;
    }

    // Unary operator -
    /// Inverts the sign of all elements in matrix.
    /// @return Matrix of same input type.
    [[nodiscard]] auto operator-() const {
        Matrix<T> result = *this;
        result._invert_sign();
        return result;
    }

    // Matrix + Matrix
    /// Add 2 matrices elementwise
    /// @return Matrix of common promoted type
    template <typename U>
    [[nodiscard]] auto operator+(const Matrix<U> &other) const {
        using R = std::common_type_t<T, U>;

        if (_rows != other.row_count() || _cols != other.column_count()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions!");
        }
        Matrix<R> result(_rows, _cols);

        if (_data.size() < 100 * 100) {
#pragma omp parallel for collapse(2)
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result.at(i, j) = this->at(i, j) + other.at(i, j);
                }
            }
        } else {
#pragma omp parallel for schedule(static, 1024)
            for (size_t idx = 0; idx < _data.size(); ++idx) {
                size_t i = idx / _cols;
                size_t j = idx % _cols;
                result.at(i, j) = this->at(i, j) + other.at(i, j);
            }
        }
        return result;
    }

    // Matrix + Scalar
    /// Add a scalar to each element of matrix.
    /// @return Matrix of common promoted type
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] auto operator+(const U &scalar) const {
        using R = std::common_type_t<T, U>;

        Matrix<R> result(_rows, _cols);

        std::transform(_data.begin(), _data.end(), result.data().begin(),
                       [scalar](const T &value) { return value + scalar; });
        return result;
    }

    // Scalar + Matrix
    /// Add a scalar to each element of matrix.
    /// @return Matrix of common promoted type
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] friend auto operator+(const U &scalar,
                                        const Matrix<T> &matrix) {
        return matrix + scalar;
    }

    // Matrix - Matrix
    /// Subtract 2 matrices elementwise.
    /// Elements of first matrix - elements of second matrix
    /// @return Matrix of common promoted type
    template <typename U>
    [[nodiscard]] auto operator-(const Matrix<U> &other) const {
        if (_rows != other.row_count() || _cols != other.column_count()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions!");
        }

        using R = std::common_type_t<T, U>;
        Matrix<R> result(_rows, _cols);

        if (_data.size() < 100 * 100) {
#pragma omp parallel for collapse(2)
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result.at(i, j) = this->at(i, j) - other.at(i, j);
                }
            }
        } else {
#pragma omp parallel for schedule(static, 1024)
            for (size_t idx = 0; idx < _data.size(); ++idx) {
                size_t i = idx / _cols;
                size_t j = idx % _cols;
                result.at(i, j) = this->at(i, j) - other.at(i, j);
            }
        }
        return result;
    }

    // Matrix - Scalar
    /// Subtract a scalar from each element of matrix.
    /// @return Matrix of common promoted type
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] auto operator-(const U &scalar) const {
        using R = std::common_type_t<T, U>;

        Matrix<R> result(_rows, _cols);
        std::transform(_data.begin(), _data.end(), result.data().begin(),
                       [scalar](const T &value) { return value - scalar; });
        return result;
    }

    // Scalar - Matrix
    /// Subtract each element of matrix from a scalar.
    /// @return Matrix of common promoted type
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] friend auto operator-(const U &scalar,
                                        const Matrix<T> &matrix) {
        using R = std::common_type_t<T, U>;

        Matrix<R> result(matrix._rows, matrix._cols);
        std::transform(matrix._data.begin(), matrix._data.end(),
                       result.data().begin(),
                       [scalar](const T &value) { return scalar - value; });
        return result;
    }

    // Matrix * Matrix
    /// Standard algebraic matrix multiplication.
    /// @return Matrix of common promoted type
    template <typename U>
    [[nodiscard]] auto operator*(const Matrix<U> &other) const {
        if (_cols != other.row_count()) {
            throw std::invalid_argument("Matrix dimensions do not match!");
        }
        using R = std::common_type_t<T, U>;

        const size_t a_rows = _rows;
        const size_t b_cols = other.column_count();
        const size_t a_cols = _cols;
        Matrix<R> result(_rows, b_cols);

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
                            const T a_ik = a_data[(i * a_cols) + k];
                            const size_t b_offset = k * b_cols;
                            const size_t c_offset = i * b_cols;

#pragma omp simd
                            for (size_t j = jj; j < j_end; ++j) {
                                c_data[c_offset + j] +=
                                    a_ik * b_data[b_offset + j];
                            }
                        }
                    }
                }
            }
        }
        return result;
    }

    // Matrix * Scalar
    /// Multiply each element of matrix by a scalar.
    /// @return Matrix of common promoted type
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] auto operator*(const U &scalar) const {
        Matrix<std::common_type_t<T, U>> result(_rows, _cols);

        std::transform(_data.begin(), _data.end(), result._data.begin(),
                       [scalar](const T &value) { return value * scalar; });
        return result;
    }

    // Scalar * Matrix
    /// Multiply each element of matrix by a scalar.
    /// @return Matrix of common promoted type
    template <typename U>
        requires(std::is_arithmetic_v<U>)
    [[nodiscard]] friend auto operator*(const U &scalar,
                                        const Matrix<T> &matrix) {
        return matrix * scalar;
    }

    // Matrix * Vector -> Vector
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

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                result.at(i) += static_cast<R>(this->at(i, j)) *
                                static_cast<R>(other.at(j));
            }
        }

        return result;
    }

    // Debugging and printing
    void print() const {
        if constexpr (std::is_floating_point_v<T>) {
            std::cout << std::setprecision(5);
        }
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                std::cout << this->at(i, j) << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::fixed;
    }
};

/// Creates new identity matrix.
/// @return Matrix of given type.
template <typename T> Matrix<T> inline identity_matrix(size_t size) {
    Matrix<T> result(size, size);
    result.make_identity();
    return result;
}

/// Creates new matrix filled with ones.
/// @return Matrix of given type.
template <typename U> inline Matrix<U> ones(size_t rows, size_t cols) {
    Matrix<U> result(rows, cols);
    result.fill(U(1));
    return result;
}
/// Checks if 2 matrices are loosely equal.
template <typename T, typename U>
[[nodiscard]] constexpr bool loosely_equal(const Matrix<T> &first,
                                           const Matrix<U> &second) {
    size_t n = first.row_count();
    size_t m = first.column_count();
    if (n != second.row_count() || m != second.column_count()) {
        return false;
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (!is_close(first.at(i, j), second.at(i, j), 1e-10)) {
                return false;
            }
        }
    }
    return true;
}

} // namespace maf

#include "CholeskyDecomposition.hpp"
#include "MatrixCheckers.hpp"
#include "MatrixMethods.hpp"
#include "PLU.hpp"

#endif
