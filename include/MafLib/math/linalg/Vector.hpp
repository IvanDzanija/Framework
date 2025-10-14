#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include "MafLib/main/GlobalHeader.hpp"
#include "MafLib/math/linalg/Matrix.hpp"

#define BLOCK_SIZE 128

namespace maf {
template <typename T> class Vector {
  private:
    enum Orientation { ROW, COLUMN };
    Orientation _orientation;
    std::vector<T> _data;

    void _invert_sign() {
        for (auto &element : _data) {
            element = -element;
        }
    }

  public:
    Vector() : _orientation(COLUMN) {}

    // Construct from raw data
    /// Constructor from data pointed to.
    /// Elements are copied over from pointer to pointer + size
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
    /// Construct emptpy vector with given size
    Vector(size_t size, Orientation orientation = COLUMN)
        : _orientation(orientation) {
        if (size == 0) {
            throw std::invalid_argument(
                "Vector size must be greater than zero.");
        }
        _data.resize(size);
    }

    // Construct from std::vector with copying
    /// Constructor from std::vector
    /// Elements of std::vector are copied over
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

    // Construct from std::vector with move semantics
    /// Constructor from std::vector.
    /// Elements of std::vector are moved over.
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
    /// Constructor from std::array.
    /// Elements of std::array are copied over.
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
    [[nodiscard]] size_t size() const noexcept { return _data.size(); }
    [[nodiscard]] Orientation orientation() const noexcept {
        return _orientation;
    }

    T &at(size_t index) { return _data.at(index); }
    const T &at(size_t index) const { return this->_data.at(index); }

    T &operator[](size_t index) { return this->_data.at(index); }
    const T &operator[](size_t index) const { return this->_data.at(index); }

    // Checkers

    /// Null vector
    /// @return true if all values of the vector are/close to 0, false otherwise
    bool is_null() {
        for (const T &val : _data) {
            if (!is_close(val, 0)) {
                return false;
            }
        }
        return true;
    }

    // Operators

    bool operator==(const Vector &other) const;

    /// Vector + Vector
    /// @return Resultant vector using standard algebraic addition method
    Vector<T> operator+(const Vector &other) const {
        if (_orientation != other._orientation ||
            _data.size() != other._data.size()) {
            throw std::invalid_argument(
                "Vectors must be same orientation and size!");
        }
        size_t n = _data.size();
        Vector<T> result(n);
        for (size_t i = 0; i < n; ++i) {
            result.at(i) = _data.at(i) + other._data.at(i);
        }
        return result;
    }

    /// Vector + Scalar
    /// @return Extended vector
    Vector<T> operator+(const T &scalar) {
        Vector<T> result(this->_data.size());
        std::transform(this->_data.begin(), this->_data.end(),
                       result._data.begin(),
                       [scalar](const T &value) { return value + scalar; });
        return result;
    }

    /// Scalar + Vector
    /// It's recommended to use Vector + Scalar instead.
    /// @return Extended vector
    template <class U>
    friend Vector<U> operator+(const U &scalar, const Vector<U> &vec);

    // Vector - Vector
    Vector<T> operator-(const Vector &other) const {
        if (this->_orientation != other._orientation ||
            this->_data.size() != other._data.size()) {
            throw std::invalid_argument(
                "Vectors must be same orientation and size!");
        }
        size_t n = this->_data.size();
        Vector<T> result(n);
        for (size_t i = 0; i < n; ++i) {
            result.at(i) = this->_data.at(i) - other._data.at(i);
        }
        return result;
    }

    // Vector - Scalar
    Vector<T> operator-(const T &scalar) {
        Vector<T> result(this->_data.size());
        std::transform(this->_data.begin(), this->_data.end(),
                       result._data.begin(),
                       [scalar](const T &value) { return value - scalar; });
        return result;
    }

    // Scalar - Vector
    template <class U>
    friend Vector<U> operator-(const U &scalar, const Vector<U> &vec);

    // Vector * Scalar
    Vector<T> operator*(const T &scalar) const {
        Vector<T> result(this->_data.size());

        std::transform(this->_data.begin(), this->_data.end(),
                       result._data.begin(),
                       [scalar](const T &value) { return value * scalar; });
        return result;
    }

    // Scalar * Vector
    template <class U>
    friend Vector<U> operator*(const U &scalar, const Vector<U> &vec);

    // Vector * Vector -> Matrix
    template <typename U> auto operator*(const Vector<U> &other) const;

    Matrix<T> operator*(const Matrix<T> &other) const {
        size_t n = this->size();
        size_t m = other.row_count();
        switch (this->_orientation) {
        case Vector<T>::COLUMN: {
            switch (m) {
            case 1:
            default:
                throw std::invalid_argument(
                    "Vector dimensions do not match! Maybe you are looking for "
                    "vector dot product.");
            }
            Matrix<T> result(n, m);
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

            T result = 0;
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < m; ++j) {
                    result += this->at(i) * other.at(j);
                }
            }
            return Matrix<T>(1, 1, {result});
        }
    }

    // Methods

    // Inplace fill
    void fill(T value) { std::fill(_data.begin(), _data.end(), value); }

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
        for (T &val : _data) {
            val /= norm;
        }
    }

    // Inplace transpose
    void transpose() { _orientation = (_orientation == COLUMN) ? ROW : COLUMN; }

    // Create new transposed vector
    Vector<T> transposed() const noexcept;

    T dot_product(const Vector<T> &other) const {
        T result = 0;
        for (size_t i = 0; i < this->size(); ++i) {
            for (size_t j = 0; j < other.size(); ++j) {
                result += this->at(i) * other.at(j);
            }
        }
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

template <class U> Vector<U> operator+(const U &scalar, const Vector<U> &vec) {
    return vec + scalar;
}

template <class U> Vector<U> operator-(const U &scalar, const Vector<U> &vec) {
    return (vec - scalar).invert_sign();
}

template <class U> Vector<U> operator*(const U &scalar, const Vector<U> &vec) {
    return vec * scalar;
}

template <class U>
Matrix<U> operator*(const Matrix<U> &lhs, const Vector<U> &rhs) {
    switch (rhs._orientation) {
    case Vector<U>::COLUMN:
        // Implement logic
        break;
    default:
        // Implement logic
    }
}

} // namespace maf

#endif
