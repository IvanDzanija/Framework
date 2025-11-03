#ifndef POLYNOMIAL_CONSTRUCTORS_H
#define POLYNOMIAL_CONSTRUCTORS_H
#pragma once
#include "Polynomial.hpp"

/**
 * @file MatrixConstructors.hpp
 * @brief Contains implementations for the constructors of the Matrix<T> class.
 *
 * This file is intended to be included at the *end* of Matrix.hpp and
 * should not be included directly anywhere else.
 */
namespace maf::math {
// Constructs an uninitialized polynomial of size rows x cols.
template <typename T>
Polynomial<T>::Polynomial(size_t degree) : _degree(degree) {
    if (degree == 0) {
        throw std::invalid_argument(
            "Polynomial degree must be greater than zero.");
    }

    _coeffs.resize(degree);
}

// Constructs a polynomial from a raw data pointer.
template <typename T>
Polynomial<T>::Polynomial(size_t degree, T *data) : _degree(degree) {
    if (degree == 0) {
        throw std::invalid_argument(
            "Polynomial degree must be greater than zero!");
    }
    if (data == nullptr) {
        throw std::invalid_argument("Data pointer cannot be null!");
    }

    _coeffs.assign(data, data + degree);
}

// Constructs from a std::vector
template <typename T>
Polynomial<T>::Polynomial(size_t degree, const std::vector<T> &coeffs)
    : _degree(degree) {
    if (degree == 0) {
        throw std::invalid_argument(
            "Polynomial degree must be greater than zero.");
    }

    if (coeffs.size() != degree) {
        throw std::invalid_argument(
            "Number of coefficients must match the degree of the polynomial");
    }

    _coeffs.assign(coeffs.begin(), coeffs.end());
}

// Constructs from a std::array
template <typename T>
template <typename U, size_t N>
Polynomial<T>::Polynomial(size_t degree, const std::array<U, N> &coeffs)
    : _degree(degree) {
    if (degree == 0) {
        throw std::invalid_argument(
            "Polynomial degree must be greater than zero.");
    }

    if (coeffs.size() != degree) {
        throw std::invalid_argument(
            "Number of coefficients must match the degree of the polynomial");
    }

    _coeffs.assign(coeffs.begin(), coeffs.end());
}

// Constructs from a std::initializer_list
template <typename T>
template <typename U>
Polynomial<T>::Polynomial(size_t degree, std::initializer_list<U> coeffs)
    : _degree(degree) {
    if (degree == 0) {
        throw std::invalid_argument(
            "Polynomial degree must be greater than zero.");
    }

    if (static_cast<size_t>(coeffs.size()) != degree) {
        throw std::invalid_argument(
            "Number of coefficients must match the degree of the polynomial");
    }
    _coeffs.assign(coeffs.begin(), coeffs.end());
}

} // namespace maf::math

#endif
