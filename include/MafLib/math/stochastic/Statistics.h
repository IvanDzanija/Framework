#ifndef STATISTICS_H
#define STATISTICS_H
#pragma once
#include "../linalg/Containers.hpp"

// TODO: this needs to be adapted to the new Containers
namespace math {
template <typename T> double mean(const Vector<T> &data) {
    double sum = 0.0;
    for (const T &value : data) {
        sum += value;
    }
    return sum / data.size();
}

template <typename T>
double covariance(const Vector<T> &x, const Vector<T> &y) {
    size_t n = x.size();
    if (n != y.size()) {
        throw std::invalid_argument("Dimension mismatch.");
    }
    double mean_x = mean(x);
    double mean_y = mean(y);
    double cov = 0.0;
    for (size_t i = 0; i < n; ++i) {
        cov += (x.at(i) - mean_x) * (y.at(i) - mean_y);
    }
    return cov / (n - 1);
}

// Use this if the means are saved already
template <typename T>
double covariance(const std::vector<T> &x, T mean_x, const std::vector<T> &y,
                  T mean_y) {
    size_t n = x.size();
    if (n != y.size()) {
        throw std::invalid_argument("Dimension mismatch.");
    }
    //	double mean_x = mean(x);
    //	double mean_y = mean(y);
    double cov = 0.0;

    for (size_t i = 0; i < n; ++i) {
        cov += (x.at(i) - mean_x) * (y.at(i) - mean_y);
    }
    return cov / (n - 1);
}

} // namespace math

#endif
