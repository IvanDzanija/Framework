#ifndef MATRIX_H
#define MATRIX_H
#pragma once
#include "MafLib/main/GlobalHeader.hpp"

#define BLOCK_SIZE 128

namespace maf {

template <typename T> class Vector;
template <typename T> class Matrix;

template <typename T> class Matrix {
  private:
    size_t _rows;
    size_t _cols;
    std::vector<T> _data;

    [[nodiscard]] bool _is_valid_index(size_t row, size_t col) const {
        return row < _rows && col < _cols;
    }

    [[nodiscard]] size_t _get_index(size_t row, size_t col) const {
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
    template <typename U>
    Matrix(size_t rows, size_t cols, const std::vector<U> &data)
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
    template <typename U>
    Matrix(size_t rows, size_t cols, const std::vector<std::vector<U>> &data)
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

    [[nodiscard]] size_t row_count() const { return _rows; }
    [[nodiscard]] size_t column_count() const { return _cols; }
    [[nodiscard]] size_t size() const { return _data.size(); }

    T &at(size_t row, size_t col) { return _data.at(_get_index(row, col)); }

    const T &at(size_t row, size_t col) const {
        return _data.at(_get_index(row, col));
    }

    // Checkers
    /// Checks if the matrix is square.
    [[nodiscard]] bool is_square() const;

    /// Checkes if the matrix is symmetric.
    [[nodiscard]] bool is_symmetric() const;

    /// Checks if the matrix is upper triangular.
    [[nodiscard]] bool is_upper_triangular() const;

    /// Checks if the matrix is lower triangular.
    [[nodiscard]] bool is_lower_triangular() const;

    /// Checks if the matrix is diagonal.
    [[nodiscard]] bool is_diagonal() const;

    /// Checks if the matrix is singular.
    /// This is equivalent to:
    /// 1) Matrix A doesn't have an inverse.
    /// 2) Rank (A) < n
    /// 3) Determinant(A) = 0
    [[nodiscard]] bool is_singular() const;

    // Methods
    /// Fills the current matrix with $value.
    void fill(T value);

    /// Converts the matrix to indentity matrix.
    void make_identity();

    /// Inplace transpose of matrix
    /// Currently works only for square matrices
    void transpose();

    /// Creates new transposed matrix
    /// @return Transposed matrix
    [[nodiscard]] Matrix<T> transposed() const;

    // Operators

    /// Checks if 2 matrices are exactly same,
    /// use loosely_equal if working with floats.
    [[nodiscard]] bool operator==(const Matrix &other) const;

    /// Add 2 matrices elementwise
    /// @return Matrix of common promoted type

    template <typename U>
    [[nodiscard]] auto operator+(const Matrix<U> &other) const;

    /// Add a scalar to each element of matrix.
    /// @return Matrix of common promoted type
    template <typename U> [[nodiscard]] auto operator+(const U &scalar) const;

    // Scalar + Matrix
    template <class U>
    friend Matrix<U> operator+(const U &scalar, const Matrix<U> &matrix);

    // Matrix - Matrix
    Matrix<T> operator-(const Matrix &other) const {
        if (_rows != other.row_count() || _cols != other.column_count()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions!");
        }
        Matrix<T> result(_rows, _cols);

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
    Matrix<T> operator-(const T &scalar) const {
        Matrix<T> result(_rows, _cols);

        std::transform(_data.begin(), _data.end(), result._data.begin(),
                       [scalar](const T &value) { return value - scalar; });
        return result;
    }

    // Scalar - Matrix
    template <class U>
    friend Matrix<U> operator-(const U &scalar, const Matrix<U> &matrix);

    // Matrix * Scalar
    Matrix<T> operator*(const T &scalar) const {
        Matrix<T> result(_rows, _cols);

        std::transform(_data.begin(), _data.end(), result._data.begin(),
                       [scalar](const T &value) { return value * scalar; });
        return result;
    }

    // Scalar * Matrix
    template <class U>
    friend Matrix<U> operator*(const U &scalar, const Matrix<T> &matrix);

    // Matrix * Vector
    template <class U> friend Matrix<T> operator*(const Vector<U> &other);

    // Matrix * Matrix
    Matrix<T> operator*(const Matrix &other) const {
        if (_cols != other._rows) {
            throw std::invalid_argument("Matrix dimensions do not match!");
        }

        Matrix<T> result(_rows, other._cols);

        const T *a_data = this->_data.data();
        const T *b_data = other._data.data();
        T *c_data = result._data.data();

        const size_t a_rows = _rows;
        const size_t b_cols = other._cols;
        const size_t a_cols = _cols;

        result.fill(T(0));

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
                            const T a_ik = a_data[i * a_cols + k];
                            //                    if (is_close(a_ik, T(0)))
                            //                    {
                            // continue; // Skip zero elements
                            //                   }
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

    // Additional computing methods
    Matrix decompose_Cholesky();
};

template <class U>
Matrix<U> operator+(const U &scalar, const Matrix<U> &matrix) {
    return matrix + scalar;
}

template <class U>
Matrix<U> operator-(const U &scalar, const Matrix<U> &matrix) {
    Matrix ret = matrix - scalar;
    ret._invert_sign();
    return ret;
}

template <class U>
Matrix<U> operator*(const U &scalar, const Matrix<U> &matrix) {
    return matrix * scalar;
}

template <class U> inline Matrix<U> zeros(size_t rows, size_t cols) {
    return Matrix<U>(rows, cols);
}

template <class U> inline Matrix<U> ones(size_t rows, size_t cols) {
    Matrix<U> result(rows, cols);
    result.fill(U(1));
    return result;
}

template <class U> Matrix<U> inline identity(size_t size) {
    Matrix<U> result(size, size);
    result.make_identity();
    return result;
}
} // namespace maf

#include "MatrixCheckers.hpp"
#include "MatrixMethods.hpp"
#include "MatrixOperators.hpp"
#endif
