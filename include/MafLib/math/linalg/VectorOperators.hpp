#include "Vector.hpp"
#include <type_traits>

namespace maf {

// Equality operator
template <typename T> bool Vector<T>::operator==(const Vector &other) const {
    if (this->_orientation != other._orientation ||
        this->size() != other.size()) {
        return false;
    }
    return _data == other._data;
}

// Vector + Vector
template <typename T>
template <typename U>

auto Vector<T>::operator+(const Vector<U> &other) const {
    using R = std::common_type_t<T, U>;

    if (_orientation != other._orientation ||
        _data.size() != other._data.size()) {
        throw std::invalid_argument(
            "Vectors must be same orientation and size!");
    }
    size_t n = _data.size();
    Vector<R> result(n);
    for (size_t i = 0; i < n; ++i) {
        result.at(i) = _data.at(i) + other._data.at(i);
    }
    return result;
}

// Vector + Scalar
template <typename T>
template <typename U>

auto Vector<T>::operator+(const U &scalar) const {
    using R = std::common_type_t<T, U>;

    Vector<R> result(this->_data.size());
    std::transform(this->_data.begin(), this->_data.end(), result._data.begin(),
                   [scalar](const T &value) { return value + scalar; });
    return result;
}

// Scalar + Vector
template <typename T, typename U>
auto operator+(const U &scalar, const Vector<T> &vec) {
    return vec + scalar;
}

// Vector - Vector
template <typename T>
template <typename U>
auto Vector<T>::operator-(const Vector<U> &other) const {
    using R = std::common_type_t<T, U>;

    if (this->_orientation != other._orientation ||
        this->_data.size() != other._data.size()) {
        throw std::invalid_argument(
            "Vectors must be same orientation and size!");
    }
    size_t n = size();
    Vector<R> result(n);
    for (size_t i = 0; i < n; ++i) {
        result.at(i) = this->_data.at(i) - other._data.at(i);
    }
    return result;
}

// Vector - Scalar
template <typename T>
template <typename U>
auto Vector<T>::operator-(const U &scalar) const {
    using R = std::common_type_t<T, U>;

    Vector<R> result(this->_data.size());
    std::transform(this->_data.begin(), this->_data.end(), result._data.begin(),
                   [scalar](const T &value) { return value - scalar; });
    return result;
}

// Scalar - Vector
template <typename T, typename U>
auto operator-(const U &scalar, const Vector<T> &vec) {
    using R = std::common_type_t<T, U>;

    Vector<R> result(vec.size());
    std::transform(vec._data.begin(), vec._data.end(), result._data.begin(),
                   [scalar](const T &value) { return scalar - value; });
    return result;
}

// Vector * Matrix -> Vector
template <typename T>
template <typename U>
auto Vector<T>::operator*(const Matrix<U> &other) const {
    using R = std::common_type_t<T, U>;

    size_t n = this->size();
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

    Vector<R> result(r, std::vector<R>(r), Vector<T>::ROW);
    for (size_t i = 0; i < r; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result.at(i) += this->at(j) * other.at(j, i);
        }
    }
    return result;
}

// Vector * Vector -> Matrix
template <typename T>
template <typename U>
auto Vector<T>::operator*(const Vector<U> &other) const {
    std::cout << "THIS IS AN OUTER PRODUCT OPERATOR!" << std::endl;

    using R = std::common_type_t<T, U>;
    size_t n = _data.size();
    size_t m = other.size();
    if (_orientation == other._orientation) {
        // 1x1 x 1x1 vector multiplication
        if (n == 1 && m == 1) {
            return Matrix(1, 1, {this->_data.at(0) * other._data.at(0)});
        }
        throw std::invalid_argument(
            "Vector dimensions do not match! Maybe you are looking for "
            "vector dot product.");
    }
    switch (_orientation) {
    case Vector<T>::COLUMN: {
        Matrix<R> result(n, m);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                result.at(i, j) = this->at(i) * other.at(j);
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
                result += this->at(i) * other.at(j);
            }
        }
        return Matrix<R>(1, 1, {result});
    }
}

// Vector * Vector -> Scalar
template <typename T>
template <typename U>
auto Vector<T>::dot_product(const Vector<U> &other) const {
    std::common_type_t<T, U> result = 0;
    for (size_t i = 0; i < this->size(); ++i) {
        for (size_t j = 0; j < other.size(); ++j) {
            result += this->at(i) * other.at(j);
        }
    }
    return result;
}

} // namespace maf
