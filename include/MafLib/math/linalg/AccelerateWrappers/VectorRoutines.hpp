#ifndef ACCELERATE_VECTOR_ROUTINES_H
#define ACCELERATE_VECTOR_ROUTINES_H

#pragma once
#include "AccelerateWrapper.hpp"
#include "MafLib/main/GlobalHeader.hpp"

namespace maf::math::acc {

// R = Alpha * X + Y (double)
[[nodiscard]] inline std::vector<double> daxpy(const std::vector<double>& x,
                                               const std::vector<double>& y,
                                               int32 alpha = 1) {
    std::vector<double> result(y);
    cblas_daxpy(x.size(), alpha, x.data(), 1, result.data(), 1);
    return result;
}

// Y = Alpha * X + Y (double)
inline void inplace_daxpy(const std::vector<double>& x,
                          std::vector<double>& y,
                          int32 alpha = 1) {
    cblas_daxpy(x.size(), alpha, x.data(), 1, y.data(), 1);
}

// R = Alpha * X + Y (float)
[[nodiscard]] inline std::vector<float> saxpy(const std::vector<float>& x,
                                              const std::vector<float>& y,
                                              int32 alpha = 1) {
    std::vector<float> result(y);
    cblas_saxpy(x.size(), alpha, x.data(), 1, result.data(), 1);
    return result;
}

// Y = Alpha * X + Y (float)
inline void inplace_saxpy(const std::vector<float>& x,
                          std::vector<float>& y,
                          int32 alpha = 1) {
    cblas_saxpy(x.size(), alpha, x.data(), 1, y.data(), 1);
}

// R = Alpha * X + Y (int32)
[[nodiscard]] inline std::vector<int> vaddi(const std::vector<int>& x,
                                            const std::vector<int>& y) {
    std::vector<int> result(y);
    vDSP_vaddi(x.data(), 1, y.data(), 1, result.data(), 1, x.size());
    return result;
}

// Y = Alpha * X + Y (int32)
inline void inplace_vaddi(const std::vector<int>& x, std::vector<int>& y) {
    vDSP_vaddi(x.data(), 1, y.data(), 1, y.data(), 1, x.size());
}

// R = Alpha * X
[[nodiscard]] inline std::vector<float> sscal(const std::vector<float>& x,
                                              int32 alpha = 1) {
    std::vector<float> result(x.begin(), x.end());
    cblas_sscal(x.size(), alpha, result.data(), 1);
    return result;
}

// X = Alpha * X
inline void inplace_sscal(std::vector<float>& x, int32 alpha = 1) {
    cblas_sscal(x.size(), alpha, x.data(), 1);
}

}  // namespace maf::math::acc
#endif
