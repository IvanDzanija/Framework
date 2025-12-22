#ifndef VECTOR_OPERATORS_H
#define VECTOR_OPERATORS_H
#pragma once
#include "Vector.hpp"

namespace maf::math {

// Equality operator
template <Numeric T>
bool Vector<T>::operator==(const Vector& other) const {
    if (_orientation != other._orientation || size() != other.size()) {
        return false;
    }
    return _data == other._data;
}

// Vector + Vector
template <Numeric T>
template <Numeric U>
auto Vector<T>::operator+(const Vector<U>& other) const {
    using R = std::common_type_t<T, U>;

    if (_orientation != other.orientation() || _data.size() != other.data().size()) {
        throw std::invalid_argument("Vectors must be same orientation and size!");
    }

    auto fallback = [&](auto& res) {
        for (size_t i = 0; i < _data.size(); ++i) {
            res[i] = static_cast<R>(_data[i]) + static_cast<R>(other[i]);
        }
    };

#if defined(__APPLE__) && defined(ACCELERATE_AVAILABLE)
    if constexpr (std::is_same_v<T, R>) {
        Vector<R> result(other);
        if constexpr (std::is_same_v<R, float>) {
            acc::inplace_saxpy(_data, result.data());
        } else if constexpr (std::is_same_v<R, double>) {
            acc::inplace_daxpy(_data, result.data());
        } else if constexpr (std::is_same_v<R, int32>) {
            acc::inplace_vaddi(_data, result.data());
        } else {
            fallback(result);
        }
        return result;
    } else if constexpr (std::is_same_v<U, R>) {
        Vector<R> result(*this);
        if constexpr (std::is_same_v<R, float>) {
            acc::inplace_saxpy(other.data(), result.data());
        } else if constexpr (std::is_same_v<R, double>) {
            acc::inplace_daxpy(other.data(), result.data());
        } else if constexpr (std::is_same_v<R, int32>) {
            acc::inplace_vaddi(other.data(), result.data());
        } else {
            fallback(result);
        }
        return result;
    }

#else
    Vector<R> result;
    result.data().reserve(_data.size());
    for (size_t i = 0; i < n; ++i) {
        result[i] = static_cast<R>(_data[i]) + static_cast<R>(other._data[i]);
    }
    return result;
#endif
}

// Vector + Scalar
template <Numeric T>
template <Numeric U>
auto Vector<T>::operator+(const U& scalar) const {
    using R = std::common_type_t<T, U>;

    size_t n = _data.size();
    Vector<R> result(n);
    R r_scalar = static_cast<R>(scalar);
    for (size_t i = 0; i < n; ++i) {
        result.at(i) = static_cast<R>(_data.at(i)) + r_scalar;
    }
    return result;
}

// Scalar + Vector
template <Numeric T, Numeric U>
auto operator+(const U& scalar, const Vector<T>& vec) {
    return vec + scalar;
}

// Vector - Vector
template <Numeric T>
template <Numeric U>
auto Vector<T>::operator-(const Vector<U>& other) const {
    using R = std::common_type_t<T, U>;

    if (_orientation != other._orientation || _data.size() != other._data.size()) {
        throw std::invalid_argument("Vectors must be same orientation and size!");
    }
    size_t n = size();
    Vector<R> result(n);
    for (size_t i = 0; i < n; ++i) {
        result.at(i) = static_cast<R>(_data.at(i)) - static_cast<R>(other._data.at(i));
    }
    return result;
}

// Vector - Scalar
template <Numeric T>
template <Numeric U>
auto Vector<T>::operator-(const U& scalar) const {
    using R = std::common_type_t<T, U>;

    size_t n = _data.size();
    Vector<R> result(n);
    R r_scalar = static_cast<R>(scalar);
    for (size_t i = 0; i < n; ++i) {
        result.at(i) = static_cast<R>(_data.at(i)) - r_scalar;
    }
    return result;
}

// Scalar - Vector
template <Numeric T, Numeric U>
auto operator-(const U& scalar, const Vector<T>& vec) {
    using R = std::common_type_t<T, U>;

    size_t n = vec._data.size();
    Vector<R> result(n);
    R r_scalar = static_cast<R>(scalar);
    for (size_t i = 0; i < n; ++i) {
        result.at(i) = r_scalar - static_cast<R>(vec._data.at(i));
    }
    return result;
}

// Vector * Matrix -> Vector
template <Numeric T>
template <Numeric U>
auto Vector<T>::operator*(const Matrix<U>& other) const {
    using R = std::common_type_t<T, U>;

    size_t n = size();
    size_t m = other.row_count();
    size_t r = other.column_count();

    if (_orientation == COLUMN) {
        throw std::invalid_argument(
            "Invalid multiplication: column Vector * Matrix. "
            "Did you mean Matrix * Vector?");
    }
    if (n != m) {
        throw std::invalid_argument("Dimensions do not match!");
    }

    Vector<R> result(r, std::vector<R>(r), ROW);
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result.at(i) += at(j) * other.at(j, i);
        }
    }
    return result;
}

// Vector * Vector -> Matrix
template <Numeric T>
template <Numeric U>
auto Vector<T>::operator*(const Vector<U>& other) const {
    std::cout << "THIS IS AN OUTER PRODUCT OPERATOR!" << std::endl;

    using R = std::common_type_t<T, U>;
    size_t n = _data.size();
    size_t m = other.size();
    if (_orientation == other._orientation) {
        // 1x1 x 1x1 vector multiplication
        if (n == 1 && m == 1) {
            return Matrix(1, 1, {_data.at(0) * other._data.at(0)});
        }
        throw std::invalid_argument(
            "Vector dimensions do not match! Maybe you are looking for "
            "vector dot product.");
    }
    switch (_orientation) {
        case COLUMN: {
            Matrix<R> result(n, m);
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    result.at(i, j) = at(i) * other.at(j);
                }
            }
            return result;
        }

        default:
            std::cout << "This results in a 1x1 matrix. Consider using vector "
                         "dot product."
                      << std::endl;

            R result = 0;
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    result += at(i) * other.at(j);
                }
            }
            return Matrix<R>(1, 1, {result});
    }
}

// Vector * Vector -> Scalar
template <Numeric T>
template <Numeric U>
auto Vector<T>::dot_product(const Vector<U>& other) const {
    std::common_type_t<T, U> result = 0;
    for (size_t i = 0; i < size(); ++i) {
        for (size_t j = 0; j < other.size(); ++j) {
            result += at(i) * other.at(j);
        }
    }
    return result;
}

}  // namespace maf::math

#endif
