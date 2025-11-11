#include "MafLib/main/GlobalHeader.hpp"
#include "MatrixTests.cpp"
#include "VectorTests.cpp"

int main() {
    MatrixTests().run_all_tests();
    VectorTests().run_all_tests();
}
