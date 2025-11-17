#pragma once
#include "MafLib/main/GlobalHeader.hpp"

namespace maf::test {
class ITest {
private:
    uint32 _passed = 0;
    uint32 _failed = 0;

public:
    virtual ~ITest() = default;

    virtual int run_all_tests() = 0;

    void print_summary() const {
        std::cout << "\n=== Checks Summary ===\n"
                  << "Passed: " << _passed << "\n"
                  << "Failed: " << _failed << "\n"
                  << "====================\n";
    }

protected:
    void assert_true(bool condition,
                     const std::string& msg,
                     const char* file,
                     int line) {
        if (!condition) {
            std::cerr << "[FAIL] " << file << ":" << line << " → " << msg << std::endl;
            ++_failed;
        } else {
            std::cout << "[PASS] " << msg << std::endl;
            ++_passed;
        }
    }
};

#define ASSERT_TRUE(cond) assert_true((cond), __func__, __FILE__, __LINE__)
}  // namespace maf::test
