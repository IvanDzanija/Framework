#ifndef VECTOR_OPERATORS_H
#define VECTOR_OPERATORS_H
#pragma once
#include "Vector.hpp"

namespace maf::math {

//=============================================================================
// BLAS LEVEL 1 ROUTINES
//=============================================================================

// Equality operator
template <Numeric T>
[[nodiscard]] constexpr bool Vector<T>::operator==(const Vector& other) const noexcept {
    if (_orientation != other._orientation || size() != other.size()) {
        return false;
    }
    return _data == other._data;
}

// Unary minus sign, creates a copy
template <Numeric T>
[[nodiscard]] auto Vector<T>::operator-() const noexcept {
    Vector<T> result(*this);
    result._invert_sign();
    return result;
}

// Vector + Vector
template <Numeric T>
template <Numeric U>
[[nodiscard]] auto Vector<T>::operator+(const Vector<U>& other) const {
    using R = std::common_type_t<T, U>;

    if (_orientation != other.orientation() || _data.size() != other.data().size()) {
        throw std::invalid_argument("Vectors must be same orientation and size!");
    }

    size_t n = _data.size();
    Vector<R> result(n, _orientation);
    if (n > OMP_LINEAR_LIMIT) {
        #pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) + static_cast<R>(other[i]);
        }
    } else {
        #pragma omp simd
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) + static_cast<R>(other[i]);
        }
    }
    return result;
}

// Vector + Scalar
template <Numeric T>
template <Numeric U>
[[nodiscard]] auto Vector<T>::operator+(const U& scalar) const noexcept {
    using R = std::common_type_t<T, U>;

    R r_scalar = static_cast<R>(scalar);
    size_t n = _data.size();

    Vector<R> result(n, _orientation);
    if (n > OMP_LINEAR_LIMIT) {
        #pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) + r_scalar;
        }
    } else {
        #pragma omp simd
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) + r_scalar;
        }
    }
    return result;
}

/**
 * @brief Element-wise scalar addition (scalar + Vector).
 * @tparam U An arithmetic scalar type.
 * @param scalar The scalar value.
 * @param vec The vector.
 * @return A new Vector of the common, promoted type.
 */
template <Numeric T, Numeric U>
[[nodiscard]] auto operator+(const U& scalar, const Vector<T>& vec) noexcept {
    return vec + scalar;
}

// Vector - Vector
template <Numeric T>
template <Numeric U>
[[nodiscard]] auto Vector<T>::operator-(const Vector<U>& other) const {
    using R = std::common_type_t<T, U>;

    if (_orientation != other.orientation() || _data.size() != other.size()) {
        throw std::invalid_argument("Vectors must be same orientation and size!");
    }

    size_t n = _data.size();
    Vector<R> result(n, _orientation);
    if (n > OMP_LINEAR_LIMIT) {
        #pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) - static_cast<R>(other[i]);
        }
    } else {
        #pragma omp simd
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) - static_cast<R>(other[i]);
        }
    }
    return result;
}

// Vector - Scalar
template <Numeric T>
template <Numeric U>
auto Vector<T>::operator-(const U& scalar) const noexcept {
    using R = std::common_type_t<T, U>;

    R r_scalar = static_cast<R>(scalar);
    size_t n = _data.size();

    Vector<R> result(n, _orientation);
    if (n > OMP_LINEAR_LIMIT) {
        #pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) - r_scalar;
        }
    } else {
        #pragma omp simd
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) - r_scalar;
        }
    }
    return result;
}

/**
 * @brief Element-wise scalar subtraction (scalar - Vector).
 * @tparam U An arithmetic scalar type.
 * @param scalar The scalar value from which elements are subtracted.
 * @param vec The vector.
 * @return A new Vector of the common, promoted type.
 */
template <Numeric T, Numeric U>
auto operator-(const U& scalar, const Vector<T>& vec) noexcept {
    using R = std::common_type_t<T, U>;

    R r_scalar = static_cast<R>(scalar);
    size_t n = vec.size();

    Vector<R> result(n, vec.orientation());
    if (n > OMP_LINEAR_LIMIT) {
        #pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            result[i] = r_scalar - static_cast<R>(vec[i]);
        }
    } else {
        #pragma omp simd
        for (size_t i = 0; i < n; ++i) {
            result[i] = r_scalar - static_cast<R>(vec[i]);
        }
    }
    return result;
}

// Vector * Scalar
template <Numeric T>
template <Numeric U>
[[nodiscard]] auto Vector<T>::operator*(const U& scalar) const noexcept {
    using R = std::common_type_t<T, U>;

    R r_scalar = static_cast<R>(scalar);
    size_t n = _data.size();

    Vector<R> result(n, _orientation);
    if (n > OMP_LINEAR_LIMIT) {
        #pragma omp parallel for
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) * r_scalar;
        }
    } else {
        #pragma omp simd
        for (size_t i = 0; i < n; ++i) {
            result[i] = static_cast<R>(_data[i]) * r_scalar;
        }
    }
    return result;
}

/**
 * @brief Element-wise scalar multiplication (scalar * Vector).
 * @tparam U An arithmetic scalar type.
 * @param scalar The scalar value.
 * @param vec The vector.
 * @return A new Vector of the common, promoted type.
 */
template <Numeric T, Numeric U>
[[nodiscard]] auto operator*(const U& scalar, const Vector<T>& vec) noexcept {
    return vec * scalar;
}

// TODO: Check if makes sense to use BLAS on routines below

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
