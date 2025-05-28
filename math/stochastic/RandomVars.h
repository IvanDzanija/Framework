#ifndef RANDOMVARS_H
#define RANDOMVARS_H
#pragma once
#include "../linalg/Containers.hpp"

namespace math {
template <typename T>
Vector<T> correlated_shocks(const Matrix<T> &L, const Vector<T> &Z) {
    size_t n = Z.size();
    Vector<T> Y(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k <= i; ++k) {
            Y.at(i) += L.at(i, k) * Z.at(k);
        }
    }
    return Y;
}
} // namespace math
#endif
