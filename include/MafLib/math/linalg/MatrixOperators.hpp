#pragma once
#include "Matrix.hpp"
#include <type_traits>

namespace maf {

template <typename T> bool Matrix<T>::operator==(const Matrix &other) const {
    if (_rows != other._rows || _cols != other._cols) {
        return false;
    }
    return _data == other._data;
}

// Matrix + Matrix
template <typename T>
template <typename U>
auto Matrix<T>::operator+(const Matrix<U> &other) const {
    if (_rows != other.row_count() || _cols != other.column_count()) {
        throw std::invalid_argument("Matrices have to be of same dimensions!");
    }
    Matrix<std::common_type_t<T, U>> result(_rows, _cols);

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
template <typename T>
template <typename U>
auto Matrix<T>::operator+(const U &scalar) const {
    Matrix<std::common_type_t<T, U>> result(_rows, _cols);

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
        throw std::invalid_argument("Matrices have to be of same dimensions!");
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
                            c_data[c_offset + j] += a_ik * b_data[b_offset + j];
                        }
                    }
                }
            }
        }
    }
    return result;
}
} // namespace maf
