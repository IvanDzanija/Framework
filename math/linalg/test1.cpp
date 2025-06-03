#include "Containers.hpp"
#include <chrono>
#include <iostream>

const int n = 1024;
int main(void) {
    math::Matrix<double> m1(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            m1.at(i, j) = i + j;
        }
    }

    math::Matrix<double> m2(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            m2.at(i, j) = i + j;
        }
    }
    auto start = std::chrono::high_resolution_clock::now();

    math::Matrix<double> m3 = m1 * m2;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
    return 0;
}
