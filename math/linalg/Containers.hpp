#ifndef CONTAINERS_H
#define CONTAINERS_H

#pragma once
#include <array>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <omp.h>
#include <optional>
#include <stdexcept>
#include <vector>

const int OUT_OF_BOUNDS = -1;
const int NOT_SQUARE = -2;

const double EPSILON = 1e-6;
template <typename T> bool is_close(T v1, T v2, double epsilon = EPSILON) {
    return std::abs(v1 - v2) < epsilon;
}

namespace math {
template <typename T> class Matrix {
  private:
    size_t _rows;
    size_t _cols;
    size_t _size;
    T *_data;

    std::optional<bool> _is_singular;

    inline bool _is_valid_index(size_t row, size_t col) const {
        return row < _rows && col < _cols;
    }
    inline size_t _get_index(size_t row, size_t col) const {
        if (!_is_valid_index(row, col)) {
            throw std::out_of_range("Index out of bounds.");
        }
        return row * _cols + col;
    }

    inline void _invert_sign() {
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = -_data[i];
        }
    }

  public:
    // Constructors and destructors

    // Default constructor
    Matrix() {
        _rows = 0;
        _cols = 0;
        _size = 0;
        _data = nullptr;
    }

    ~Matrix() {
        if (_data != nullptr) {
            delete[] _data;
        }
    }

    // Construct from raw data
    Matrix(size_t rows, size_t cols, T *data) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero!");
        }
        if (data == nullptr) {
            throw std::invalid_argument("Data pointer cannot be null!");
        }
        _rows = rows;
        _cols = cols;
        _size = rows * cols;
        _data = new T[_size];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = data[i];
        }
    }

    // Construct empty of size rows x cols
    Matrix(size_t rows, size_t cols) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }
        _rows = rows;
        _cols = cols;
        _size = rows * cols;
        _data = new T[_size];

        // Default initialization
        for (size_t i = 0; i < rows * cols; ++i) {
            _data[i] = T();
        }
    }

    // Copy constructor
    Matrix(const Matrix &other) {
        _rows = other._rows;
        _cols = other._cols;
        _size = other._size;
        _data = new T[_size];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = other._data[i];
        }
    }

    // Move constructor
    Matrix(Matrix &&other) noexcept {
        _rows = other._rows;
        _cols = other._cols;
        _size = other._size;
        _data = other._data;

        // Reset the moved-from object
        other._rows = 0;
        other._cols = 0;
        other._size = 0;
        other._data = nullptr;
    }

    // Assignment operator
    Matrix &operator=(const Matrix &other) {
        if (this != &other) {
            if (_data != nullptr) {
                delete[] _data;
            }
            _rows = other._rows;
            _cols = other._cols;
            _size = other._size;
            _data = new T[_size];
            for (size_t i = 0; i < _size; ++i) {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }

    // Move assignment operator
    Matrix &operator=(Matrix &&other) noexcept {
        if (this != &other) {
            if (_data != nullptr) {
                delete[] _data;
            }
            _rows = other._rows;
            _cols = other._cols;
            _size = other._size;
            _data = other._data;

            // Reset the moved-from object
            other._rows = 0;
            other._cols = 0;
            other._size = 0;
            other._data = nullptr;
        }
        return *this;
    }

    // Constructors for std::vector and std::array
    template <typename U>
    Matrix(size_t rows, size_t cols, const std::vector<U> &data) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }
        if (data.size() != rows * cols) {
            throw std::invalid_argument(
                "Data size does not match matrix size.");
        }
        _rows = rows;
        _cols = cols;
        _size = rows * cols;
        _data = new T[_size];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = static_cast<T>(data.at(i));
        }
    }

    template <typename U>
    Matrix(size_t rows, size_t cols, const std::vector<std::vector<U>> &data) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }
        if (data.size() != rows || data.at(0).size() != cols) {
            throw std::invalid_argument(
                "Data size does not match matrix size.");
        }
        _rows = rows;
        _cols = cols;
        _size = rows * cols;
        _data = new T[_size];
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                _data[_get_index(i, j)] = static_cast<T>(data.at(i).at(j));
            }
        }
    }

    template <typename U, size_t N>
    Matrix(size_t rows, size_t cols, const std::array<U, N> &data) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument(
                "Matrix dimensions must be greater than zero.");
        }
        if (N != rows * cols) {
            throw std::invalid_argument(
                "Data size does not match matrix size.");
        }
        _rows = rows;
        _cols = cols;
        _size = N;
        _data = new T[_size];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = static_cast<T>(data.at(i));
        }
    }

    T &at(int row, int col) {
        if (!_is_valid_index(row, col)) {
            throw std::out_of_range("Matrix index out of bounds");
        }
        return _data[_get_index(row, col)];
    }

    const T &at(int row, int col) const {
        if (!_is_valid_index(row, col)) {
            throw std::out_of_range("Matrix index out of bounds");
        }
        return _data[_get_index(row, col)];
    }

    // Getters
    size_t rowsSize() const { return _rows; }
    size_t colsSize() const { return _cols; }

    // Equality operator
    bool operator==(const Matrix &other) const {
        if (_rows != other._rows || _cols != other._cols) {
            return false;
        }
        for (size_t i = 0; i < _size; ++i) {
            if (_data[i] != other._data[i]) {
                return false;
            }
        }
        return true;
    }

    // Checkers

    bool is_square() const { return _rows == _cols; }

    bool is_symmetric() const {
        if (!is_square()) {
            throw std::invalid_argument(
                "Matrix must be square to check symmetry.");
        }
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = i + 1; j < _cols; ++j) {
                if (!is_close(this->at(i, j), this->at(j, i))) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_upper_triangular() const {
        for (size_t i = 1; i < _rows; ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (!is_close(this->at(i, j), static_cast<T>(0))) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_lower_triangular() const {
        for (size_t i = 0; i < _rows - 1; ++i) {
            for (size_t j = i + 1; j < _cols; ++j) {
                if (!is_close(this->at(i, j), static_cast<T>(0))) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_diagonal() const {
        if (!is_square()) {
            throw std::invalid_argument(
                "Matrix must be square to check diagonality.");
        }
        return is_upper_triangular() && is_lower_triangular();
    }

    bool is_singular() {
        if (_is_singular.has_value()) {
            return _is_singular.value();
        }
        if (!is_square()) {
            throw std::invalid_argument(
                "Non-square matrices are always singular.");
        }
        // Needs logic
        // For now, we will assume the matrix is not singular

        _is_singular = false; // Placeholder logic
        return _is_singular.value();
    }

    // Methods

    // Inplace fill
    void fill(T value) {
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = value;
        }
    }

    // Set to indentity
    void make_identity() {
        if (!is_square()) {
            throw std::invalid_argument(
                "Only square matrices can be set to identity!");
        }
        for (size_t i = 0; i < _rows; ++i) {
            this->at(i, i) = 1;
        }
    }

    // Inplace transpose
    void transpose() {
        if (!is_square()) {
            throw std::invalid_argument("Matrix must be square to transpose.");
        }

#pragma omp parallel for
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = i + 1; j < _cols; ++j) {
                std::swap(const_cast<Matrix *>(this)->at(i, j),
                          const_cast<Matrix *>(this)->at(j, i));
            }
        }
    }

    // New transposed matrix
    const Matrix transposed() const {
        T *result = new T[_size];
#pragma omp parallel for if (_size > 100 * 100)
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                result[j * _rows + i] = this->at(i, j);
            }
        }
        Matrix ret(_cols, _rows, result);
        delete[] result;
        return ret;
    }

    // Operators

    // Matrix + Matrix
    Matrix<T> operator+(const Matrix &other) const {
        if (_rows != other.rowsSize() || _cols != other.colsSize()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions!");
        }
        Matrix<T> result(_rows, _cols);

        if (_size < 100 * 100) {
#pragma omp parallel for collapse(2)
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result.at(i, j) = this->at(i, j) + other.at(i, j);
                }
            }
        } else {
#pragma omp parallel for schedule(static, 1024)
            for (size_t idx = 0; idx < _size; ++idx) {
                size_t i = idx / _cols;
                size_t j = idx % _cols;
                result.at(i, j) = this->at(i, j) + other.at(i, j);
            }
        }
        return result;
    }

    // Matrix + Scalar
    Matrix<T> operator+(const T &scalar) const {
        T *result = new T[_size];

        if (_size < 100 * 100) {
            for (size_t i = 0; i < _size; ++i) {
                result[i] = _data[i] + scalar;
            }
        } else {
#pragma omp parallel for simd schedule(static, 1024)
            for (size_t i = 0; i < _size; ++i) {
                result[i] = _data[i] + scalar;
            }
        }

        Matrix<T> ret(_rows, _cols, result);
        delete[] result;
        return ret;
    }

    // Scalar + Matrix
    template <class U>
    friend Matrix<U> operator+(const U &scalar, const Matrix<U> &matrix);

    // Matrix - Matrix
    Matrix<T> operator-(const Matrix &other) const {
        if (_rows != other.rowsSize() || _cols != other.colsSize()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions!");
        }
        Matrix<T> result(_rows, _cols);

        if (_size < 100 * 100) {
#pragma omp parallel for collapse(2)
            for (size_t i = 0; i < _rows; ++i) {
                for (size_t j = 0; j < _cols; ++j) {
                    result.at(i, j) = this->at(i, j) - other.at(i, j);
                }
            }
        } else {
#pragma omp parallel for schedule(static, 1024)
            for (size_t idx = 0; idx < _size; ++idx) {
                size_t i = idx / _cols;
                size_t j = idx % _cols;
                result.at(i, j) = this->at(i, j) - other.at(i, j);
            }
        }
        return result;
    }

    // Matrix - Scalar
    Matrix<T> operator-(const T &scalar) const {
        T *result = new T[_size];

        if (_size < 100 * 100) {
            for (size_t i = 0; i < _size; ++i) {
                result[i] = _data[i] - scalar;
            }
        } else {
#pragma omp parallel for simd schedule(static, 1024)
            for (size_t i = 0; i < _size; ++i) {
                result[i] = _data[i] - scalar;
            }
        }

        Matrix<T> ret(_rows, _cols, result);
        delete[] result;
        return ret;
    }

    // Scalar - Matrix
    template <class U>
    friend Matrix<U> operator-(const U &scalar, const Matrix<U> &matrix);

    // Matrix * Scalar
    Matrix<T> operator*(const T &scalar) const {
        T *result = new T[_size];

        if (_size < 100 * 100) {
            for (size_t i = 0; i < _size; ++i) {
                result[i] = _data[i] * scalar;
            }
        } else {
#pragma omp parallel for simd schedule(static, 1024)
            for (size_t i = 0; i < _size; ++i) {
                result[i] = _data[i] * scalar;
            }
        }

        Matrix<T> ret(_rows, _cols, result);
        delete[] result;
        return ret;
    }

    // Scalar * Matrix
    template <class U>
    friend Matrix<U> operator*(const U &scalar, const Matrix<U> &matrix);

    // Matrix * Matrix
    Matrix<T> operator*(const Matrix &other) const {
        if (_cols != other._rows) {
            throw std::invalid_argument("Matrix dimensions do not match!");
        }

        Matrix<T> result(_rows, other._cols);
        const T *a_data = this->_data;
        const T *b_data = other._data;
        T *c_data = result._data;

        const size_t a_rows = _rows;
        const size_t b_cols = other._cols;
        const size_t a_cols = _cols;
        const size_t block_size = 64;

        std::fill(c_data, c_data + a_rows * b_cols, T(0));

#pragma omp parallel for collapse(2) if (a_rows * b_cols > 10000)
        for (size_t ii = 0; ii < a_rows; ii += block_size) {
            for (size_t jj = 0; jj < b_cols; jj += block_size) {
                for (size_t kk = 0; kk < a_cols; kk += block_size) {

                    // Process block
                    const size_t i_end = std::min(ii + block_size, a_rows);
                    const size_t j_end = std::min(jj + block_size, b_cols);
                    const size_t k_end = std::min(kk + block_size, a_cols);

                    for (size_t i = ii; i < i_end; ++i) {
                        for (size_t k = kk; k < k_end; ++k) {
                            const T a_ik = a_data[i * a_cols + k];
                            const size_t b_row_offset = k * b_cols;
                            const size_t c_row_offset = i * b_cols;

#pragma omp simd
                            for (size_t j = jj; j < j_end; ++j) {
                                c_data[c_row_offset + j] +=
                                    a_ik * b_data[b_row_offset + j];
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

template <typename T> class Vector {
  private:
    enum Orientation { ROW, COLUMN };
    Orientation _orientation;
    T *_data;

  public:
};
} // namespace math

#endif
