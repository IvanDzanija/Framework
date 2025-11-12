cmake_minimum_required(VERSION 3.20)

project(MafLib
    VERSION 1.0.0
    DESCRIPTION "Experimental C++ framework"
    LANGUAGES CXX
)

# Require Clang
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "This project requires Clang, but ${CMAKE_CXX_COMPILER_ID} was found. Please use a Clang compiler.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Library setup
# ---------------------------------------------------------------------------
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ---------------------------------------------------------------------------
# OpenMP cross-platform setup
# ---------------------------------------------------------------------------
set(OPENMP_FOUND FALSE)

if(APPLE)
    # macOS: use Homebrew libomp
    find_program(HOMEBREW brew)
    if(HOMEBREW)
        execute_process(COMMAND ${HOMEBREW} --prefix libomp
            OUTPUT_VARIABLE LIBOMP_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(COMMAND ${HOMEBREW} --prefix llvm
            OUTPUT_VARIABLE LLVM_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE)

        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${LIBOMP_PREFIX}/include")
        set(OpenMP_omp_LIBRARY "${LIBOMP_PREFIX}/lib/libomp.dylib")
        set(OPENMP_FOUND TRUE)
        message(STATUS "Using Homebrew OpenMP from ${LIBOMP_PREFIX}")
    else()
        message(WARNING "Homebrew not found — please install libomp via 'brew install libomp'.")
    endif()

elseif(WIN32)
    # Windows: manually locate LLVM OpenMP
    set(LLVM_ROOT "C:/Program Files/LLVM")
    set(OpenMP_CXX_FLAGS "-Xclang -fopenmp -I${LLVM_ROOT}/include")

    if(EXISTS "${LLVM_ROOT}/lib/libomp.lib")
        set(OpenMP_omp_LIBRARY "${LLVM_ROOT}/lib/libomp.lib")
        set(OPENMP_FOUND TRUE)
        message(STATUS "Using OpenMP from ${LLVM_ROOT}/lib/libomp.lib")
    elseif(EXISTS "${LLVM_ROOT}/lib/libiomp5md.lib")
        set(OpenMP_omp_LIBRARY "${LLVM_ROOT}/lib/libiomp5md.lib")
        set(OPENMP_FOUND TRUE)
        message(STATUS "Using Intel OpenMP runtime from ${LLVM_ROOT}/lib/libiomp5md.lib")
    else()
        message(WARNING "OpenMP library not found under '${LLVM_ROOT}/lib' — parallel sections will be disabled.")
    endif()

else()
    # Linux: system libomp-dev
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        set(OPENMP_FOUND TRUE)
        set(OpenMP_omp_LIBRARY OpenMP::OpenMP_CXX)
        message(STATUS "Using system OpenMP package")
    else()
        message(WARNING "OpenMP not found — install with 'sudo apt install libomp-dev'.")
    endif()
endif()

if(OPENMP_FOUND)
    target_compile_options(${PROJECT_NAME} INTERFACE ${OpenMP_CXX_FLAGS})
    target_link_libraries(${PROJECT_NAME} INTERFACE ${OpenMP_omp_LIBRARY})
    message(STATUS "✅ OpenMP enabled")
else()
    message(WARNING "⚠️  OpenMP support disabled (library not found)")
endif()

# ---------------------------------------------------------------------------
# Target configuration
# ---------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# Clang-specific warnings and optimizations
target_compile_options(${PROJECT_NAME}
    INTERFACE
        -fcolor-diagnostics
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g -O0 -fsanitize=address -fsanitize=undefined>
        $<$<CONFIG:Release>:-O3 -ffast-math -march=native -funroll-loops -DNDEBUG>
)

target_link_options(${PROJECT_NAME}
    INTERFACE
        $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined>
)

# ---------------------------------------------------------------------------
# Installation
# ---------------------------------------------------------------------------
include(GNUInstallDirs)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
