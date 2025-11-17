#ifndef VECTOR_CONSTRUCTORS_H
#define VECTOR_CONSTRUCTORS_H
#pragma once
#include "Vector.hpp"

/**
 * @file VectorConstructors.hpp
 * @brief Contains implementations for the constructors of the Vector<T> class.
 *
 * This file is intended to be included at the *end* of Vector.hpp and
 * should not be included directly anywhere else.
 */
namespace maf::math {
// Constructs an uninitialized vector of size size.
template <Numeric T>
[[nodiscard]] Vector<T>::Vector(size_t size, Orientation orientation)
    : _orientation(orientation) {
    if (size == 0) {
        throw std::invalid_argument("Vector size must be greater than zero.");
    }
    _data.resize(size);
}

// Constructs a vector from a raw data pointer.
template <Numeric T>
template <Numeric U>
[[nodiscard]] Vector<T>::Vector(size_t size, const U* data, Orientation orientation)
    : _orientation(orientation) {
    if (size == 0) {
        throw std::invalid_argument("Vector size must be greater than zero!");
    }
    if (data == nullptr) {
        throw std::invalid_argument("Data pointer cannot be null!");
    }

    _data.assign(data, data + size);
}

// Constructs from a std::vector, copy constructor
template <Numeric T>
template <Numeric U>
[[nodiscard]] Vector<T>::Vector(size_t size,
                                const std::vector<U>& data,
                                Orientation orientation)
    : _orientation(orientation) {
    if (size == 0) {
        throw std::invalid_argument("Vector size must be greater than zero.");
    }

    if (data.size() != size) {
        throw std::invalid_argument("Data size does not match vector size.");
    }
    _data = data;
}

// Constructs from a std::vector, move constructor
template <Numeric T>
Vector<T>::Vector(size_t size, std::vector<T>&& data, Orientation orientation)
    : _orientation(orientation) {
    if (size == 0) {
        throw std::invalid_argument("Vector size must be greater than zero.");
    }

    if (data.size() != size) {
        throw std::invalid_argument("Data size does not match vector size.");
    }

    _data = std::move(data);
}

// Constructs from a std::array, copy constructor
template <Numeric T>
template <Numeric U, size_t N>
[[nodiscard]] Vector<T>::Vector(size_t size,
                                const std::array<U, N>& data,
                                Orientation orientation)
    : _orientation(orientation) {
    if (size == 0) {
        throw std::invalid_argument("Vector size must be greater than zero.");
    }
    if (N != size) {
        throw std::invalid_argument("Data size does not match vector size.");
    }

    _data.assign(data.begin(), data.end());
}

}  // namespace maf::math

#endif
