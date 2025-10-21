# tests/math/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(TEST_NAME LinAlgTests)

add_executable(${TEST_NAME}
    LinAlgTest.cpp
)

target_link_libraries(${TEST_NAME}
    PRIVATE
        MafLib::MafLib
)

target_include_directories(${TEST_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
