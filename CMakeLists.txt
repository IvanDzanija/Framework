cmake_minimum_required(VERSION 3.20)

project(MafLib
    VERSION 1.0.0
    DESCRIPTION "Experimental C++ framework for HPC"
    LANGUAGES CXX
)

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "This project requires Clang, but ${CMAKE_CXX_COMPILER_ID} was found. Please use a Clang compiler.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


if(APPLE)
    # Try to find libomp via Homebrew
    find_program(HOMEBREW brew)
    if(HOMEBREW)
        execute_process(COMMAND ${HOMEBREW} --prefix libomp
            OUTPUT_VARIABLE LIBOMP_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(COMMAND ${HOMEBREW} --prefix llvm
            OUTPUT_VARIABLE LLVM_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE)

        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${LIBOMP_PREFIX}/include")
        set(OpenMP_CXX_LIB_NAMES omp)
        set(OpenMP_omp_LIBRARY ${LIBOMP_PREFIX}/lib/libomp.dylib)
    endif()
endif()

find_package(OpenMP REQUIRED)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        OpenMP::OpenMP_CXX
)

# Clang-specific optimizations and flags
target_compile_options(${PROJECT_NAME}
    INTERFACE
        -fcolor-diagnostics
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-g -O0 -fsanitize=address -fsanitize=undefined>
        $<$<CONFIG:Release>:-O2 -ffast-math -march=native -funroll-loops -DNDEBUG>
)

target_link_options(${PROJECT_NAME}
    INTERFACE
        $<$<CONFIG:Debug>:-fsanitize=address -fsanitize=undefined>
)

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

option(BUILD_EXAMPLES "Build examples" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
