#include "MafLib/main/GlobalHeader.hpp"
#include "MatrixTests.cpp"
#include "VectorTests.cpp"

int main() {
    std::cout << "=== Running Matrix tests ===" << std::endl;
    auto matrix_tests = MatrixTests();
    matrix_tests.run_all_tests();
    matrix_tests.print_summary();

    VectorTests().run_all_tests();
}
