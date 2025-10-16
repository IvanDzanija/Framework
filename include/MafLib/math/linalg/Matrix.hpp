#ifndef MATRIX_H
#define MATRIX_H
#pragma once
#include "MafLib/main/GlobalHeader.hpp"

#define BLOCK_SIZE 128

namespace maf {
template <typename T> class Vector;

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

    // [[nodiscard]] std::vector<T> &data() noexcept { return _data; }
    [[nodiscard]] const std::vector<T> &data() const noexcept { return _data; }
    [[nodiscard]] size_t row_count() const noexcept { return _rows; }
    [[nodiscard]] size_t column_count() const noexcept { return _cols; }
    [[nodiscard]] size_t size() const noexcept { return _data.size(); }

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

    // Matrix + Matrix
    /// Add 2 matrices elementwise
    /// @return Matrix of common promoted type
    template <typename U>
    [[nodiscard]] auto operator+(const Matrix<U> &other) const;

    // Matrix + Scalar
    /// Add a scalar to each element of matrix.
    /// @return Matrix of common promoted type
    template <typename U> [[nodiscard]] auto operator+(const U &scalar) const;

    // Scalar + Matrix
    /// Add a scalar to each element of matrix.
    /// @return Matrix of common promoted type
    template <typename U>
    friend auto operator+(const U &scalar, const Matrix<T> &matrix);

    // Matrix - Matrix
    /// Subtract 2 matrices elementwise.
    /// Elements of first matrix - elements of second matrix
    /// @return Matrix of common promoted type
    template <typename U>
    [[nodiscard]] auto operator-(const Matrix<U> &other) const;

    // Matrix - Scalar
    /// Subtract a scalar from each element of matrix.
    /// @return Matrix of common promoted type
    template <typename U> [[nodiscard]] auto operator-(const U &scalar) const;

    // Scalar - Matrix
    /// Subtract each element of matrix from a scalar.
    /// @return Matrix of common promoted type
    template <typename U>
    friend auto operator-(const U &scalar, const Matrix<T> &matrix);

    // Matrix * Scalar
    /// Multiply each element of matrix by a scalar.
    /// @return Matrix of common promoted type
    template <typename U> [[nodiscard]] auto operator*(const U &scalar) const;

    // Scalar * Matrix
    /// Multiply each element of matrix by a scalar.
    /// @return Matrix of common promoted type
    template <typename U>
    friend auto operator*(const U &scalar, const Matrix<T> &matrix);

    // Matrix * Vector
    template <typename U>
    [[nodiscard]] auto operator*(const Vector<U> &other) const;

    // Matrix * Matrix
    template <typename U>
    [[nodiscard]] auto operator*(const Matrix<U> &other) const;

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

} // namespace maf

#include "MatrixCheckers.hpp"
#include "MatrixMethods.hpp"
#include "MatrixOperators.hpp"
#endif
