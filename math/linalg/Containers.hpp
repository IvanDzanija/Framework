#ifndef CONTAINERS_H
#define CONTAINERS_H

#pragma once
#include "../../main/GlobalHeader.hpp"

#define BLOCK_SIZE 128

constexpr double EPSILON = 1e-6;
template <typename T> bool is_close(T v1, T v2, double epsilon = EPSILON) {
    return std::abs(v1 - v2) < epsilon;
}

namespace math {
template <typename T> class Matrix {
  private:
    size_t _rows;
    size_t _cols;
    std::vector<T> _data;

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

    T &at(size_t row, size_t col) { return _data.at(_get_index(row, col)); }

    const T &at(size_t row, size_t col) const {
        return _data.at(_get_index(row, col));
    }

    // Getters
    size_t row_count() const { return _rows; }
    size_t column_count() const { return _cols; }
    size_t size() const { return _data.size(); }

    // Equality operator
    bool operator==(const Matrix &other) const {
        if (_rows != other._rows || _cols != other._cols) {
            return false;
        }
        return _data == other._data;
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
        // TODO
        //   Needs logic
        //   For now, we will assume the matrix is not singular
        _is_singular = false; // Placeholder logic
        return _is_singular.value();
    }

    // Methods

    // Inplace fill
    void fill(T value) { std::fill(_data.begin(), _data.end(), value); }

    // Set to identity
    void make_identity() {
        if (!is_square()) {
            throw std::invalid_argument(
                "Only square matrices can be set to identity!");
        }

        fill(T(0));
        for (size_t i = 0; i < _rows; ++i) {
            this->at(i, i) = T(1);
        }
    }

    // Inplace transpose
    void transpose() {
        if (!is_square()) {
            // Maybe change this?
            throw std::invalid_argument("Matrix must be square to transpose.");
        }

#pragma omp parallel for
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = i + 1; j < _cols; ++j) {
                std::swap(this->at(i, j), this->at(j, i));
            }
        }
    }

    // New transposed matrix
    const Matrix transposed() const {
        Matrix result(_cols, _rows);

#pragma omp parallel for if (_data.size() > 100 * 100)
        for (size_t i = 0; i < _rows; ++i) {
            for (size_t j = 0; j < _cols; ++j) {
                result.at(j, i) = this->at(i, j);
            }
        }
        return result;
    }

    // Operators

    // Matrix + Matrix
    Matrix<T> operator+(const Matrix &other) const {
        if (_rows != other.row_count() || _cols != other.column_count()) {
            throw std::invalid_argument(
                "Matrices have to be of same dimensions!");
        }
        Matrix<T> result(_rows, _cols);

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
    Matrix<T> operator+(const T &scalar) const {
        Matrix<T> result(_rows, _cols);

        std::transform(_data.begin(), _data.end(), result._data.begin(),
                       [scalar](const T &value) { return value + scalar; });
        return result;
    }

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
    friend Matrix<U> operator*(const U &scalar, const Matrix<U> &matrix);

    // Matrix * Matrix
    Matrix<T> operator*(const Matrix &other) const {
        if (_cols != other._rows) {
            throw std::invalid_argument("Matrix dimensions do not match!");
        }

        Matrix<T> result(_rows, other._cols);

        // CHANGE: Use vector data() method to get raw pointer for performance
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
                            if (is_close(a_ik, T(0))) {
                                continue; // Skip zero elements
                            }
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
    void print(void) const {
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
    Matrix decompose_Cholesky(void);
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

template <typename T> class Vector {
  private:
    enum Orientation { ROW, COLUMN };
    Orientation _orientation;
    std::vector<T> _data;

  public:
    Vector() : _orientation(COLUMN) {}

    // Construct from raw data
    Vector(size_t size, T *data, Orientation orientation = COLUMN)
        : _orientation(orientation) {
        if (size == 0) {
            throw std::invalid_argument(
                "Vector size must be greater than zero!");
        }
        if (data == nullptr) {
            throw std::invalid_argument("Data pointer cannot be null!");
        }

        _data.assign(data, data + size);
    }

    // Construct empty of size size
    Vector(size_t size, Orientation orientation = COLUMN)
        : _orientation(orientation) {
        if (size == 0) {
            throw std::invalid_argument(
                "Vector size must be greater than zero.");
        }
        _data.resize(size);
    }

    // Construct from std::vector with copying
    Vector(size_t size, const std::vector<T> &data,
           Orientation orientation = COLUMN)
        : _orientation(orientation) {
        if (size == 0) {
            throw std::invalid_argument(
                "Vector size must be greater than zero.");
        }

        if (data.size() != size) {
            throw std::invalid_argument(
                "Data size does not match vector size.");
        }

        _data = data;
    }

    // Construct from std::vector with moving
    Vector(size_t size, std::vector<T> &&data, Orientation orientation = COLUMN)
        : _orientation(orientation) {
        if (size == 0) {
            throw std::invalid_argument(
                "Vector size must be greater than zero.");
        }

        if (data.size() != size) {
            throw std::invalid_argument(
                "Data size does not match vector size.");
        }

        _data = std::move(data);
    }

    // Constructor from std::array
    template <typename U, size_t N>
    Vector(size_t size, const std::array<U, N> &data,
           Orientation orientation = COLUMN)
        : _orientation(orientation) {
        if (size == 0) {
            throw std::invalid_argument(
                "Vector size must be greater than zero.");
        }
        if (N != size) {
            throw std::invalid_argument(
                "Data size does not match vector size.");
        }

        _data.assign(data.begin(), data.end());
    }

    // Getters
    size_t size() const { return _data.size(); }
    Orientation orientation() const { return _orientation; }

    T &at(size_t index) { return _data.at(index); }
    const T &at(size_t index) const { return _data.at(index); }

    T &operator[](size_t index) { return _data.at(index); }
    const T &operator[](size_t index) const { return _data.at(index); }

    // Equality operator
    bool operator==(const Vector &other) const {
        if (_orientation != other._orientation) {
            return false;
        }
        return _data == other._data;
    }

    // Checkers

    // Null vector
    bool is_null() {
        for (const T &val : _data) {
            if (!is_close(val, 0)) {
                return true;
            }
        }
        return false;
    }

    // Methods

    // Vector norm
    T norm() const {
        T sum = 0.0;
        for (const T &val : _data) {
            sum += val * val;
        }
        return std::sqrt(sum);
    }

    // Inplace normalization
    void normalize() {
        // This will probably be faster by only calling norm() and checking if
        // norm is close to 0 since it only loops through the vector once! But
        // keeping this for clarity
        if (this->is_null()) {
            throw std::invalid_argument("Null vector can't be normalized!");
        }

        T norm = this->norm();
        for (const T &val : _data) {
            val /= norm;
        }
    }

    // Inplace transpose
    void transpose() { _orientation = (_orientation == COLUMN) ? ROW : COLUMN; }

    // Create new transposed vector
    Vector transposed() const {
        Orientation new_orientation = (_orientation == COLUMN) ? ROW : COLUMN;
        return ret(this->size(), _data, new_orientation);
    }

    // Printing and debugging
    void print() const {
        if constexpr (std::is_floating_point_v<T>) {
            std::cout << std::setprecision(5);
        }
        if (_orientation == COLUMN) {
            for (const T &val : _data) {
                std::cout << val << std::endl;
            }
        } else {
            for (const T &val : _data) {
                std::cout << val << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::fixed;
    }
};

} // namespace math

#endif
