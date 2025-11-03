# Framework

A modern, header-only C++ framework for numerical computing and linear algebra operations, designed with performance and ease of use in mind.

## Overview

Framework is a small experimental C++ library that provides essential building blocks for numerical computing applications. Built with modern C++ standards and optimized for performance, it aims to deliver a clean, intuitive API for mathematical operations commonly needed in computational finance, scientific computing, and numerical analysis.

## Features

- **Header-Only Design**: Easy integration into existing projects without complex build configurations
- **Modern C++ Standards**: Leverages C++20/C++23 features for clean, expressive code
- **CMake Integration**: Simple build system with separate debug and release configurations
- **Cross-Platform**: Works on macOS, Linux, and other Unix-like systems
- **Performance Focused**: Optimized for numerical computations with Clang compiler support
- **Comprehensive Testing**: Built-in test suite using CTest

## Project Structure

```
Framework/
├── CMakeLists.txt          # Main CMake configuration
├── include/                # Public header files
├── src/                    # Implementation files
├── examples/               # Example applications
├── tests/                  # Unit tests
└── build/                  # Build directory (generated)
    ├── clang-debug/        # Debug build artifacts
    └── clang-release/      # Release build artifacts
```

## Requirements

- **C++ Compiler**: Clang with C++20 support (or later)
- **CMake**: Version 3.15 or higher
- **Operating System**: macOS, Linux (Ubuntu with Sway), or other Unix-like systems

## Quick Start

### Building the Project

#### Debug Build

```bash
cmake --preset clang-debug && cmake --build --preset clang-debug
```

#### Release Build

```bash
cmake --preset clang-release && cmake --build --preset clang-release
```

### Running Tests

#### Debug Mode

```bash
ctest --preset clang-debug
```

#### Release Mode

```bash
ctest --preset clang-release
```

### Running Examples

After building in release mode:

```bash
./build/clang-release/examples/example
```

## Usage

Since Framework is header-only, simply include the necessary headers in your project:

```cpp
#include <framework/your_header.h>

int main() {
    // Your code here
    return 0;
}
```

## CMake Integration

To use Framework in your CMake project:

```cmake
# Add Framework to your project
add_subdirectory(path/to/Framework)

# Link against Framework
target_link_libraries(your_target PRIVATE framework)
```

## Development

### Compiler Flags

The project is configured to use Clang with optimizations:
- **Debug**: Includes debugging symbols and assertions
- **Release**: Maximum optimization levels for performance

### Code Style

- Modern C++ idioms and best practices
- STL-compliant interfaces where applicable
- Template-heavy design for compile-time optimization
- Clear, expressive naming conventions

### Contributing

Contributions are welcome! When contributing:

1. Follow the existing code style
2. Add tests for new functionality
3. Ensure all tests pass in both debug and release modes
4. Update documentation as needed

## Design Philosophy

Framework is built around several key principles:

- **Simplicity**: Clean, intuitive APIs that are easy to understand and use
- **Performance**: Compile-time optimization through templates and modern C++ features
- **Correctness**: Comprehensive testing to ensure reliability
- **Flexibility**: Modular design allowing users to include only what they need
- **Maintainability**: Clear code structure and documentation

## Use Cases

Framework is suitable for:

- Numerical analysis and scientific computing
- Financial modeling and simulations
- Linear algebra operations
- Matrix computations
- Educational projects in numerical methods
- Prototyping mathematical algorithms

## Performance

The framework is designed with performance in mind:

- Template-based design for zero-overhead abstractions
- Optimized compilation with Clang
- Expression templates for efficient mathematical operations (where applicable)
- Minimal runtime overhead

## Platform Support

Tested and supported on:

- **macOS**: AppleClang compiler
- **Linux**: Ubuntu with Clang
- **Build Tools**: CMake 3.15+

## Future Development

Potential areas for expansion:

- Additional numerical algorithms
- GPU acceleration support via OpenMP/CUDA
- Extended matrix operations
- Integration with BLAS/LAPACK for optimal performance
- Additional platform support

## Documentation

For detailed documentation:

- See header files for API documentation
- Check the `examples/` directory for usage examples
- Review tests in `tests/` for additional code samples

## License

This project is released under the MIT License. See `LICENSE` file for details.

## Author

**Ivan Dzanija**

- GitHub: [@IvanDzanija](https://github.com/IvanDzanija)

## Acknowledgments

Built as an experimental framework to explore modern C++ design patterns and numerical computing techniques.

---

**Note**: This is an experimental project under active development. APIs may change as the framework evolves.

## Getting Help

If you encounter issues or have questions:

1. Check the examples directory for usage patterns
2. Review the test suite for expected behavior
3. Open an issue on GitHub with detailed information

## Versioning

This project is currently in experimental/development phase. Semantic versioning will be adopted upon the first stable release.

---

*Last updated: November 2025*
