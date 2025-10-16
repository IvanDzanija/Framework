# Framework

Small experimental C++ framework

# Debug build

cmake --preset clang-debug && cmake --build --preset clang-debug

# Release build

cmake --preset clang-release && cmake --build --preset clang-release

# Run tests in debug mode

ctest --preset clang-debug

# Run tests in release mode

ctest --preset clang-release

# Run example

./build/clang-release/examples/example
