#ifndef MATRIX_METHODS_H
#define MATRIX_METHODS_H
#pragma once
#include "Matrix.hpp"

namespace maf::math {
// Cast and create a new matrix
template <Numeric T>
template <Numeric U>
[[nodiscard]] Matrix<U> Matrix<T>::cast() const {
  // Replace this with explicit constructor casting when implemented
  Matrix<U> result(_rows, _cols);

  const size_t n = _data.size();
  for (size_t i = 0; i < n; ++i) {
    result.data()[i] = static_cast<U>(_data[i]);
  }
  return result;
}

}  // namespace maf::math

#endif
