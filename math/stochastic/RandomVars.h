#ifndef RANDOMVARS_H
#define RANDOMVARS_H
#pragma once
#include "../linalg/Containers.hpp"

namespace math {
template <typename T>
Vector<T> correlated_shocks(const Matrix<T> &L, const Vector<T> &Z) {
    size_t n = Z.size();
    auto test = L * Z;
    test.print();
    return L * Z;
}
} // namespace math
#endif
