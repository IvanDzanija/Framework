#include "Vector.hpp"

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

} // namespace maf
