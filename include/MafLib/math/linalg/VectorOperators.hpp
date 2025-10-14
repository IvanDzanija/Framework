#include "Vector.hpp"
#include <stdexcept>
#include <type_traits>

/// Equality operator
namespace maf {

template <typename T> bool Vector<T>::operator==(const Vector &other) const {
    if (this->_orientation != other._orientation ||
        this->size() != other.size()) {
        return false;
    }
    for (size_t i = 0; i < this->size(); ++i) {
        if (!is_close(this->at(i), other.at(i))) {
            return false;
        }
    }
    return true;
}
template <typename T>
template <typename U>
auto Vector<T>::operator*(const Matrix<U> &other) const {
    size_t n = this->size();
    size_t m = other.row_count();
    switch (_orientation) {
    case Vector<T>::COLUMN: {
        if (m == 1) {
            throw std::invalid_argument(
                "You are multiplying vector(Nx1) * matrix(1*M) which is "
                "supposed "
                "to be a vector by vector multiplication!");
        }
        throw std::invalid_argument("Dimension do not match: " + size() +
                                    "x1 * " + other.row_count() + "x" +
                                    other.column_count());
    }
    default:
        Vector<std::common_type_t<T, U>> result(m, Vector<T>::ROW);
        // TODO: Finish this
    }
}

// Vector * Vector -> Matrix
template <typename T>
template <typename U>
auto Vector<T>::operator*(const Vector<U> &other) const {
    std::cout << "THIS IS AN OUTER PRODUCT OPERATOR!" << std::endl;
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
        Matrix<std::common_type_t<T, U>> result(n, m);
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

        std::common_type_t<T, U> result = 0;
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                result += this->at(i) * other.at(j);
            }
        }
        return Matrix<std::common_type_t<T, U>>(1, 1, {result});
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
