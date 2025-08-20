#include "bit_twiddling.hpp"
#include <chrono>
#include <iostream>

int main() {
    using namespace bit;

    // Test same_sign function
    std::cout << "Testing same_sign function:\n";
    std::cout << "same_sign(5, 10): " << same_sign(5, 10) << " (expected 1)\n";
    std::cout << "same_sign(-5, -10): " << same_sign(-5, -10)
              << " (expected 1)\n";
    std::cout << "same_sign(-5, 10): " << same_sign(-5, 10)
              << " (expected 0)\n";
    std::cout << "same_sign(0, 5): " << same_sign(0, 5) << " (expected 1)\n";
    std::cout << "same_sign(0, -5): " << same_sign(0, -5) << " (expected 0)\n";

    // Test is_power_of_two function
    std::cout << "\nTesting is_power_of_two function:\n";
    std::cout << "is_power_of_two(1): " << is_power_of_two(1)
              << " (expected 1)\n";
    std::cout << "is_power_of_two(2): " << is_power_of_two(2)
              << " (expected 1)\n";
    std::cout << "is_power_of_two(3): " << is_power_of_two(3)
              << " (expected 0)\n";
    std::cout << "is_power_of_two(4): " << is_power_of_two(4)
              << " (expected 1)\n";
    std::cout << "is_power_of_two(0): " << is_power_of_two(0)
              << " (expected 0)\n";
    std::cout << "is_power_of_two(-2): " << is_power_of_two(-2)
              << " (expected 0)\n";

    // Test most_significant_bit function
    std::cout << "\nTesting most_significant_bit function:\n";
    std::cout << "most_significant_bit(0): " << most_significant_bit(0)
              << " (expected 0)\n";
    std::cout << "most_significant_bit(1): " << most_significant_bit(1)
              << " (expected 1)\n";
    std::cout << "most_significant_bit(16): " << most_significant_bit(16)
              << " (expected 5)\n";
    std::cout << "most_significant_bit(255): " << most_significant_bit(255)
              << " (expected 8)\n";

    // Performance test with 1e9 iterations
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "Performance Test - 1e9 iterations\n";
    std::cout << std::string(50, '=') << "\n";

    const int iterations = 1000000000; // 1e9
    int a = 5, b = -10, n = 16;

    auto start = std::chrono::high_resolution_clock::now();

    int64_t sum = 0; // To prevent optimization of unused variables
    for (int i = 0; i < iterations; ++i) {
        volatile bool res1 = same_sign(a, b);
        volatile bool res2 = is_power_of_two(n);
        volatile size_t res3 = most_significant_bit(n);
        sum += res1 + res2 + res3;
        a += b += n += i;
    }
    std::cout << "Sum of results: " << sum << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time taken for 1e9 iterations: " << elapsed.count()
              << " seconds\n";
    std::cout << "Average time per iteration: "
              << (elapsed.count() / iterations) * 1e9 << " nanoseconds\n";

    return 0;
}
