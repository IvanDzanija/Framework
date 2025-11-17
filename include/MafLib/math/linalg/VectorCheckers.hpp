#ifndef VECTOR_CHECKERS_H
#define VECTOR_CHECKERS_H
#pragma once
#include "Vector.hpp"

/**
 * @file VectorCheckers.hpp
 * @brief Contains implementations for the checker methods of the Vector<T>
 * class.
 *
 * This file is intended to be included at the *end* of Vector.hpp and
 * should not be included directly anywhere else.
 */
namespace maf::math {
// Checks if vector is null vector
template <Numeric T>
[[nodiscard]] bool Vector<T>::is_null() const noexcept {
    for (const T& val : _data) {
        if (!is_close(val, 0)) {
            return false;
        }
    }
    return true;
}

}  // namespace maf::math

#endif
